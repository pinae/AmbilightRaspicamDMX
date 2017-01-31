#pragma once

#include "mmalincludes.h"
#include "cameracontrol.h"

class CCamera;

class CCameraOutput
{
public:
	int						Width;
	int						Height;
	MMAL_COMPONENT_T*		ResizerComponent;
	MMAL_CONNECTION_T*		Connection;
	MMAL_BUFFER_HEADER_T*	LockedBuffer;
	MMAL_POOL_T*			BufferPool;
	MMAL_QUEUE_T*			OutputQueue;
	MMAL_PORT_T*			BufferPort;

	CCameraOutput();
	~CCameraOutput();
	bool Init(int width, int height, MMAL_COMPONENT_T* input_component, int input_port_idx, bool do_argb_conversion);
	void Release();
	void OnVideoBufferCallback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
	static void VideoBufferCallback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
	int ReadFrame(void* buffer, int buffer_size);
	bool BeginReadFrame(const void* &out_buffer, int& out_buffer_size);
	void EndReadFrame();
	MMAL_POOL_T* EnablePortCallbackAndCreateBufferPool(MMAL_PORT_T* port, MMAL_PORT_BH_CB_T cb, int buffer_count);
	MMAL_COMPONENT_T* CreateResizeComponentAndSetupPorts(MMAL_PORT_T* video_output_port, bool do_argb_conversion);

};

class CCamera
{
public:

	int ReadFrame(int level, void* buffer, int buffer_size);
	bool BeginReadFrame(int level, const void* &out_buffer, int& out_buffer_size);
	void EndReadFrame(int level);
	//MMAL_PARAM_EXPOSUREMODE_T CCamera::getExposureMode();
	void CCamera::setExposureMode(MMAL_PARAM_EXPOSUREMODE_T mode);
	void CCamera::setExposureCompensation(int exposure_compensation);
	//MMAL_PARAM_AWBMODE_T CCamera::getAwbMode();
	void CCamera::setAwbMode(MMAL_PARAM_AWBMODE_T mode);
	void CCamera::setShutterSpeed(int speed_ms);
	//int CCamera::getBrightness();
	void CCamera::setBrightness(int brightness);
	//int CCamera::getContrast();
	void CCamera::setContrast(int contrast);
	//int CCamera::getISO();
	void CCamera::setISO(int iso);
	//int CCamera::getSaturation();
	void CCamera::setSaturation(int saturation);
	//int CCamera::getSharpness();
	void CCamera::setSharpness(int sharpness);

private:
	CCamera();
	~CCamera();

	bool Init(int width, int height, int framerate, int num_levels, bool do_argb_conversion);
	void Release();
	MMAL_COMPONENT_T* CreateCameraComponentAndSetupPorts();
	MMAL_COMPONENT_T* CreateSplitterComponentAndSetupPorts(MMAL_PORT_T* video_ouput_port);

	void OnCameraControlCallback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
	static void CameraControlCallback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);

	int							Width;
	int							Height;
	int							FrameRate;
	RASPICAM_CAMERA_PARAMETERS	CameraParameters;
	MMAL_COMPONENT_T*			CameraComponent;    
	MMAL_COMPONENT_T*			SplitterComponent;
	MMAL_CONNECTION_T*			VidToSplitConn;
	CCameraOutput*				Outputs[4];

	friend CCamera* StartCamera(int width, int height, int framerate, int num_levels, bool do_argb_conversion);
	friend void StopCamera();
};

CCamera* StartCamera(int width, int height, int framerate, int num_levels, bool do_argb_conversion=true);
void StopCamera();
