#pragma once
#include <wdm.h>
#include <wdf.h>

#include <initguid.h>
#include <hidport.h>

NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER, _Inout_ PWDFDEVICE_INIT);
VOID EvtIoInternalDeviceControl(_In_ WDFQUEUE, _In_ WDFREQUEST, _In_ size_t, _In_ size_t, _In_ ULONG);
NTSTATUS ioctl_hid_get_device_attributes(_In_ WDFREQUEST request);
NTSTATUS ioctl_hid_get_device_descriptor(_In_ WDFREQUEST request);
NTSTATUS ioctl_hid_get_report_descriptor(_In_ WDFREQUEST request);
NTSTATUS iotcl_hid_read_report(_In_ WDFQUEUE queue, _In_ WDFREQUEST request, OUT BOOLEAN* complete);
NTSTATUS ioctl_hid_write_report(_In_ WDFQUEUE queue, _In_ WDFREQUEST request);

typedef struct _CONTEXT {
	WDFQUEUE _queue;
} CONTEXT, * PCONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(CONTEXT, DeviceGetContext)






#define REPORT_ID_MOUSE_INPUT 0x01
#define REPORT_ID_MOUSE_OUTPUT 0x02

typedef UCHAR HID_REPORT_DESCRIPTOR, * PHID_REPORT_DESCRIPTOR;

HID_REPORT_DESCRIPTOR g_reportDescriptor[] = {
		0x05, 0x01,                         // USAGE_PAGE (Generic Desktop) 
		0x09, 0x02,                         // USAGE (Mouse)               
		0xa1, 0x01,                         // COLLECTION (Application)   
		0x85, REPORT_ID_MOUSE_INPUT,               //   REPORT_ID (Mouse)       
		0x09, 0x01,                         //   USAGE (Pointer)        
		0xa1, 0x00,                         //   COLLECTION (Physical) 
		0x05, 0x09,                         //     USAGE_PAGE (Button)
		0x19, 0x01,                         //     USAGE_MINIMUM (Button 1) 
		0x29, 0x05,                         //     USAGE_MAXIMUM (Button 5)
		0x15, 0x00,                         //     LOGICAL_MINIMUM (0)    
		0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)   
		0x75, 0x01,                         //     REPORT_SIZE (1)      
		0x95, 0x05,                         //     REPORT_COUNT (5)    
		0x81, 0x02,                         //     INPUT (Data,Var,Abs)
		0x95, 0x03,                         //     REPORT_COUNT (3)   
		0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)    
		0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
		0x26, 0xff, 0x7f,                   //     LOGICAL_MAXIMUM (32767)    
		0x75, 0x10,                         //     REPORT_SIZE (16)
		0x95, 0x01,                         //     REPORT_COUNT (1)
		0x55, 0x0F,                         //     UNIT_EXPONENT (-1)
		0x65, 0x11,                         //     UNIT (cm,SI Linear)
		0x35, 0x00,                         //     PHYSICAL_MINIMUM (0)
		0x45, 0x00,                         //     PHYSICAL_MAXIMUM (0)
		0x09, 0x30,                         //     USAGE (X)
		0x81, 0x02,                         //     INPUT (Data,Var,Abs)
		0x09, 0x31,                         //     USAGE (Y)
		0x81, 0x02,                         //     INPUT (Data,Var,Abs)
		0xc0,                               //   END_COLLECTION              
		0xc0,                               // END_COLLECTION    

		0x06, 0x00, 0xff,                    // USAGE_PAGE (Vendor Defined Page 1)
		0x09, 0x01,                          // USAGE (Vendor Usage 1)
		0xa1, 0x01,                          // COLLECTION (Application)
		0x85, REPORT_ID_MOUSE_OUTPUT,        //   REPORT_ID (1)  
		0x15, 0x00,                          //   LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (255)
		0x75, 0x08,                          //   REPORT_SIZE  (8)   - bits
		0x95, 0x06,                          //   REPORT_COUNT (6)  - Bytes
		0x09, 0x02,                          //   USAGE (Vendor Usage 1)
		0x81, 0x02,                          //   INPUT (Data,Var,Abs)
		0x95, 0x06,                          //   REPORT_COUNT (6)  - Bytes
		0x09, 0x02,                          //   USAGE (Vendor Usage 1)
		0x91, 0x02,                          //   OUTPUT (Data,Var,Abs)
		0xc0,                                // END_COLLECTION
};

HID_DESCRIPTOR g_descriptor = {
	0x09,        // length of HID descriptor
	0x21,        // descriptor type == HID 0x21
	0x0100,      // hid spec release
	0x00,        // country code == Not Specified
	0x01,        // number of HID class descriptors
	{            // DescriptorList[0]
		0x22,                             // report descriptor type 0x22
		sizeof(g_reportDescriptor)        // total length of report descriptor
	}
};

//typedef struct _REPORT {
//	BYTE reportId;
//	UCHAR button;
//	USHORT x, y;
//}REPORT;
//
//typedef struct _HEADER {
//	BYTE ReportID;
//	BYTE ReportLength;
//} HEADER;