#ifndef __HOST_HCD_H__
#define	__HOST_HCD_H__


//OTG���Ӽ��
typedef enum _USB_LINK_STATE
{
	USB_HOST_LINK, 				//U��������״̬		:����ʹ�ܣ�������ֹ��DP == 1
	USB_HOST_LINK_JITTER, 		//U����������״̬	:����ʹ�ܣ�������ֹ��DP == 1
	USB_HOST_DETECT,			//���U������״̬	:����ʹ�ܣ�������ֹ��DP == 0
	
	USB_DEVICE_DETECT,			//���PC����״̬	:������ֹ��������ֹ��DP == 1
	USB_DEVICE_LINK_JITTER,		//PC��������״̬
	USB_DEVICE_UNLINK_JITTER,  	//PC�Ͽ�����״̬
	USB_DEVICE_LINK, 			//PC������״̬		:������ֹ��������ֹ��DP == 0
	USB_DEVICE_ACTIVE, 			//PC�Ѽ���״̬		:������ֹ������ʹ�ܣ�SOF����
	USB_DEVICE_SOF_TIMEOUT		//SOF��ʱ״̬		:������ֹ��������ֹ��DP == 0

} USB_LINK_STATE;

extern BYTE gUsbLinkState;


// Endpoint number. Fixed, user should not modify.
//��ΪHOSTʱ�������Ķ˵�Ÿ�����˵�ſ��Բ���ͬ
#define	HOST_CONTROL_EP				0
#define	HOST_BULK_IN_EP				1
#define	HOST_BULK_OUT_EP			1


//pipe type
#define PIPE_TYPE_CONTROL		0
#define PIPE_TYPE_ISO_IN		1
#define PIPE_TYPE_ISO_OUT		2
#define PIPE_TYPE_INT_IN		3
#define PIPE_TYPE_INT_OUT		4
#define PIPE_TYPE_BULK_IN		5
#define PIPE_TYPE_BULK_OUT		6

//control transfer direction
#define USB_DIR_OUT				0			
#define USB_DIR_IN				1

//define OTG endpoint descriptor, used for configuration endpoint information
typedef struct _PIPE_INFO
{
	BYTE	EndpointNum	;	//endpoint number
	BYTE	PipeType;		//endpoint type: ctrl, bulk, interrupt, isochronous
	BYTE	MaxPacketSize;	//max packet size

} PIPE_INFO;

extern BOOL IsUsbIOUseToGpio; //USB IO�˿�������ͨGPIO

VOID OTGLinkCheck(VOID);

BOOL HostUsbOpen(VOID);
BOOL HostIsLink(VOID);		//���USB�豸����״̬
VOID HostUsbPortReset(VOID);
VOID HostUsbSetAddress(BYTE Address); // Set device address

//�ӿ��ƶ˵����һ�δ���
BOOL HostUsbSetupPacket(BYTE* SetupPacket, BYTE Direction, BYTE* Buf, WORD Len);

//BOOL HostSendSetup(PIPE_INFO* Pipe, BYTE* Buf, BYTE Len);
BOOL HostRcvPacket(PIPE_INFO* Pipe, BYTE* Buf, WORD Len);
BOOL HostSendPacket(PIPE_INFO* Pipe, BYTE* Buf, WORD Len);

#endif