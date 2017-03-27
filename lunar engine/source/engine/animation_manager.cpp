/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_manager.cpp
---------------------------------------------------
*/

#include "animation_manager.h"

//Dependencies
extern "C" {

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

#include "timer_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
animation_manager *animation_manager::_Instance=0;

//Protected

void animation_manager::copy(const animation_manager &_Animation_manager) {

_Animation_container=_Animation_manager._Animation_container;
_Timer_manager=_Animation_manager._Timer_manager;
}

//Ffmpeg specific

void animation_manager::init_ffmpeg() {
	av_register_all();
}

bool animation_manager::create_ffmpeganimation(animation_data *_Animation_data) {

AVFormatContext *_Format_context;
bool _File=true;

std::string _Bytes;

	//Make new animation from data file (memory)
	if (_Animation_container->load_bytes(_Animation_data,_Bytes)) {
	
	_Format_context=av_alloc_format_context();
	_Format_context->pb=av_alloc_put_byte((unsigned char*)_Bytes.c_str(),
		(int)_Bytes.size(),0,0,0,0,0);
	
	AVFormatParameters _Params={0};
	_Params.prealloced_context=1;
	
	AVProbeData _Probe_data;
	_Probe_data.filename=0;
	_Probe_data.buf=(unsigned char*)_Bytes.c_str();
	_Probe_data.buf_size=(int)_Bytes.size();
	
		if (av_open_input_stream(&_Format_context,_Format_context->pb,
			_Animation_data->filename().c_str(),av_probe_input_format(
				&_Probe_data,1),&_Params)!=0) {
		
		av_freep(_Format_context);
		_Bytes.clear();
		}
		else _File=false;
	}
	
	//Make new animation from file (filename)
	if (_Bytes.empty() && av_open_input_file(&_Format_context,
		_Animation_data->filename().c_str(),0,0,0)!=0) return false;
	
	//Retrieve stream information
	if (av_find_stream_info(_Format_context)<0) {
	
		//Close the video input
		if (_File) av_close_input_file(_Format_context);
		else av_close_input_stream(_Format_context);
	
	return false;
	}

int _Video_stream=-1;

	//Find the video stream index
	for (unsigned int i=0;i<_Format_context->nb_streams;++i) {
	
		if (_Format_context->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) {
		
		_Video_stream=i;
		break;
		}
	}
    
    //Didn't find a video stream
	if (_Video_stream==-1) {
	
		//Close the video input
		if (_File) av_close_input_file(_Format_context);
		else av_close_input_stream(_Format_context);
	
	return false;
	}

//Get a pointer to the codec context for the video stream
AVCodecContext *_Codec_context=_Format_context->streams[_Video_stream]->codec;

//Find the decoder for the video stream
AVCodec *_Codec=avcodec_find_decoder(_Codec_context->codec_id);

	//Codec decoder not found
	if (!_Codec) {
	
		//Close the video input
		if (_File) av_close_input_file(_Format_context);
		else av_close_input_stream(_Format_context);
	
	return false;
	}
	
	//Inform the codec that we can handle truncated bitstreams,
	//bitstreams where frame boundaries can fall in the middle of packets
	if (_Codec->capabilities & CODEC_CAP_TRUNCATED)
		_Codec_context->flags|=CODEC_FLAG_TRUNCATED;
	
	//Open codec
	if (avcodec_open(_Codec_context,_Codec)<0) {
	
		//Close the video input
		if (_File) av_close_input_file(_Format_context);
		else av_close_input_stream(_Format_context);
	
	return false;
	}

AVRational _Framerate=_Format_context->streams[_Video_stream]->r_frame_rate;

unsigned int _Bitdepth=(_Codec_context->pix_fmt==PIX_FMT_RGB32 ||
	_Codec_context->pix_fmt==PIX_FMT_BGR32 ||
		_Codec_context->pix_fmt==PIX_FMT_RGB32_1 ||
			_Codec_context->pix_fmt==PIX_FMT_BGR32_1 ||
				_Codec_context->pix_fmt==PIX_FMT_YUVA420P?32:24);

unsigned int _Format=(_Bitdepth==32?GL_RGBA:GL_RGB);
unsigned int _Width=_Codec_context->width;
unsigned int _Height=_Codec_context->height;

unsigned int _Glwidth=2;
unsigned int _Glheight=2;

	//Make sure each gltexture is power of 2
	while (_Glwidth<_Width) _Glwidth*=2;
	while (_Glheight<_Height) _Glheight*=2;	

unsigned int _Size=_Width*_Height;
unsigned int _Glsize=_Glwidth*_Glheight;
unsigned int _Components=_Bitdepth/8;

unsigned char *_Pixels=new unsigned char[_Size*_Components];
unsigned char *_Flip=new unsigned char[_Width*_Components];
unsigned char *_Data=new unsigned char[_Glsize*_Components];
memset(_Data,0,_Glsize*_Components); //Clear buffer with color 0,0,0,0

AVFrame *_Frame=avcodec_alloc_frame();
AVFrame *_Frame_rgb=avcodec_alloc_frame();

//Assign appropriate parts of buffer to image planes in rgb frame
avpicture_fill((AVPicture*)_Frame_rgb,_Pixels,_Bitdepth==32?
	PIX_FMT_RGB32:PIX_FMT_RGB24,_Width,_Height);

//Get a pointer to the convert context
SwsContext *_Convert_context=sws_getContext(_Width,_Height,
	_Codec_context->pix_fmt,_Width,_Height,_Bitdepth==32?
		PIX_FMT_RGB32:PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

std::vector<unsigned int> _Frames;
AVPacket _Packet;

	//Retrieve all frames from video stream
	while (av_read_frame(_Format_context,&_Packet)>=0) {
	
		//Check if packet is from the video stream
		if (_Packet.stream_index==_Video_stream) {
		
		int _Frame_finished;
		avcodec_decode_video(_Codec_context,_Frame,&_Frame_finished,
			_Packet.data,_Packet.size); //Decode video frame
		
			//Video frame found and finished
			if (_Frame_finished!=0) {
			
			sws_scale(_Convert_context,_Frame->data,_Frame->linesize,
				0,_Height,_Frame_rgb->data,_Frame_rgb->linesize); //Convert to RGB/ARGB
			
			unsigned char *_Source;
			unsigned char *_Destination;
			
				//Flip pixels vertical
				for (unsigned int i=0;i<_Height/2;++i) {
				
				_Source=_Pixels+(_Width*_Components*i);
				_Destination=_Pixels+(_Width*_Components*(_Height-1-i));
				
				memmove(_Flip,_Destination,_Width*_Components);
				memmove(_Destination,_Source,_Width*_Components);
				memmove(_Source,_Flip,_Width*_Components);
				}
				
				//Frame pixel to opengl data
				for (unsigned int i=0,_Data_index=0,_Pixel_index=0;i<_Size;++i,
					_Data_index+=_Components,_Pixel_index+=_Components) {
				
					//Row complete
					if (i>0 && i%_Width==0) {
					
						//Align pixels for power of 2 gltexture
						if (_Width<_Glwidth) _Data_index+=
							(_Glwidth-_Width)*_Components;
					}
					
					//BGRA to RGBA conversion
					if (_Bitdepth==32) {
					
					_Data[_Data_index+0]=_Pixels[_Pixel_index+2];
					_Data[_Data_index+1]=_Pixels[_Pixel_index+1];
					_Data[_Data_index+2]=_Pixels[_Pixel_index+0];
					_Data[_Data_index+3]=_Pixels[_Pixel_index+3];
					}
					else {
					
					_Data[_Data_index+0]=_Pixels[_Pixel_index+0];
					_Data[_Data_index+1]=_Pixels[_Pixel_index+1];
					_Data[_Data_index+2]=_Pixels[_Pixel_index+2];
					}
				}
			
			//Make OpenGL texture
			unsigned int _Gltexture;
			glGenTextures(1,&_Gltexture);
			glBindTexture(GL_TEXTURE_2D,(GLuint)_Gltexture);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexImage2D(GL_TEXTURE_2D,0,(GLint)_Format,_Glwidth,_Glheight,
				0,(GLenum)_Format,GL_UNSIGNED_BYTE,_Data);
			
			_Frames.push_back(_Gltexture);
			}
		}
	
	//Free the packet that was allocated by av_read_frame
	av_free_packet(&_Packet);
	}

sws_freeContext(_Convert_context); //Free convert context

av_free(_Frame_rgb); //Free the rgb frame
av_free(_Frame); //Free the frame

delete[] _Data; //Delete the image data
delete[] _Flip; //Delete the flip buffer
delete[] _Pixels; //Delete the image pixels

avcodec_close(_Codec_context); //Close the codec

	//Close the video input
	if (_File) av_close_input_file(_Format_context);
	else av_close_input_stream(_Format_context);

//Copy data from vector to array
unsigned int *_Gltexture=new unsigned int[_Frames.size()];
memcpy(_Gltexture,&_Frames[0],_Frames.size()*sizeof(unsigned int));

_Created_animation _Mycreated_animation;
_Mycreated_animation._Animation_data=_Animation_data;
_Mycreated_animation._Framerate=(float)_Framerate.num/_Framerate.den;
_Mycreated_animation._Frames=(unsigned int)_Frames.size();
_Mycreated_animation._Gltexture=_Gltexture;
_Mycreated_animation._Texture_width=_Width;
_Mycreated_animation._Texture_height=_Height;
_Mycreated_animation._Gltexture_width=_Glwidth;
_Mycreated_animation._Gltexture_height=_Glheight;
_Mycreated_animations.push_front(_Mycreated_animation); //Faster access to newly added elements
return true;
}

void animation_manager::init_animation(animation *_Animation,
	const _Created_animation &_Mycreated_animation) {

_Animation->_Frames=_Mycreated_animation._Frames;
_Animation->_Gltexture=_Mycreated_animation._Gltexture;
_Animation->_Texture_width=_Mycreated_animation._Texture_width;
_Animation->_Texture_height=_Mycreated_animation._Texture_height;
_Animation->_Gltexture_width=_Mycreated_animation._Gltexture_width;
_Animation->_Gltexture_height=_Mycreated_animation._Gltexture_height;

	//Use default framerate
	if (_Animation->_Framerate==0.0f) _Animation->framerate(
		_Mycreated_animation._Framerate);
}

void animation_manager::update_frame(animation *_Animation) {

	//Need to change frame?
	if (_Animation->_Timer->ticked()) {
	
		//Callback, frame complete
		if (_Animation->_Onframecomplete)
			(*_Animation->_Onframecomplete)(_Animation);
		
		if (_Animation->_Reverse?_Animation->_Frame==0:
			_Animation->_Frame>=_Animation->_Frames-1) {
		
			//Callback, animation complete
			if (_Animation->_Oncomplete)
				(*_Animation->_Oncomplete)(_Animation);
			
			if (_Animation->_Loop) {
			
			_Animation->stop(); //Reset
			_Animation->start();
			}
			//Don't reset, draw last frame
			else _Animation->stop(false);
		}
		else {
		
		//Do not reset time as animation::next_frame()
		
			if (_Animation->_Reverse) --_Animation->_Frame;
			else ++_Animation->_Frame;
		}
	}
}

//Public

animation_manager::animation_manager():_Animation_container(
	animation_container::instance()),_Timer_manager(timer_manager::instance()) {
	init_ffmpeg();
}

animation_manager::animation_manager(timer_manager *_Timer_manager):
	_Animation_container(animation_container::instance()),
		_Timer_manager(_Timer_manager) {
	init_ffmpeg();
}

animation_manager::animation_manager(const animation_manager &_Animation_manager):
	manager(_Animation_manager) {

init_ffmpeg();
copy(_Animation_manager);
}

animation_manager::~animation_manager() {

tidy(); //Call before clearing cache
clear_cache();
}

animation_manager &animation_manager::operator=(const animation_manager &_Animation_manager) {

	//Worth doing
	if (this!=&_Animation_manager) {
	
	manager::operator=(_Animation_manager); //Invoke base operator=
	copy(_Animation_manager);
	}

return *this;
}

animation_manager *animation_manager::instance() {
	return _Instance;
}

void animation_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()==manageable::FAILED) return;

animation *_Animation=dynamic_cast<animation*>(_Manageable);

	//Invalid type for this manager
	if (!_Animation) return;
	
	//No timer
	if (!_Animation->_Timer) {
	
	status(_Animation,manageable::FAILED);
	return;
	}
	
	//Update animation frame
	if (_Animation->_Status==manageable::OKAY) {
	
	update_frame(_Animation);
	return; //No need for more update
	}

animation_data *_Animation_data=static_cast<animation_data*>(
	_Manageable->data());

	//No data
	if (!_Animation_data) {
	
	status(_Animation,manageable::FAILED);
	return;
	}
	
	//Reuse animation
	for (_Created_animations::const_iterator _Iter=_Mycreated_animations.begin(),
		_End=_Mycreated_animations.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)._Animation_data==_Animation_data) {
		
		init_animation(_Animation,*_Iter);
		status(_Animation,manageable::OKAY); //No need for update anymore
		return;
		}
	}
	
	//Could not create ffmpeg animation
	if (!create_ffmpeganimation(_Animation_data)) {
	
	status(_Animation,manageable::FAILED);
	return;
	}

init_animation(_Animation,_Mycreated_animations.front());
update_frame(_Animation);
status(_Animation,manageable::OKAY); //No need for update anymore
}

void animation_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void animation_manager::animation_source(animation_container *_Animation_container) {
	this->_Animation_container=_Animation_container;
}

animation_container *animation_manager::animation_source() const {
	return _Animation_container;
}

//Animation functions

animation *animation_manager::create_animation(const std::string &_Name,
	bool _Enabled,bool _Visible,bool _Secured) {
	return create_animation(static_cast<animation_data*>(
		_Animation_container->data(_Name)),_Enabled,_Visible,_Secured);
}

animation *animation_manager::create_animation(const animation_data &_Animation_data,
	bool _Enabled,bool _Visible,bool _Secured) {
	return create_animation(_Animation_container->add_data(
		_Animation_data),_Enabled,_Visible,_Secured);
}

animation *animation_manager::create_animation(animation_data *_Animation_data,
	bool _Enabled,bool _Visible,bool _Secured) {

animation *_Myanimation=new animation(this,_Animation_data,
	_Timer_manager->create_timer(0.0f,_Enabled,_Secured),_Visible);
add(_Myanimation,_Secured);
return _Myanimation;
}

animation *animation_manager::create_animation(const animation &_Animation,
	bool _Secured) {

animation *_Myanimation=new animation(this,_Animation,_Animation._Timer?
	_Timer_manager->create_timer(*_Animation._Timer,_Secured):0);
add(_Myanimation,_Secured);
return _Myanimation;
}

void animation_manager::clear_cache() {

	for (_Created_animations::const_iterator _Iter=_Mycreated_animations.begin(),
		_End=_Mycreated_animations.end();_Iter!=_End;++_Iter) {
	
	const _Created_animation &_Mycreated_animation=(*_Iter); //Increase speed
	
	glDeleteTextures(_Mycreated_animation._Frames,_Mycreated_animation._Gltexture);
	delete[] _Mycreated_animation._Gltexture;
	}

_Mycreated_animations.clear();

	//Update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) status(
			*_Iter,manageable::NEED_UPDATE);
}

} //namespace