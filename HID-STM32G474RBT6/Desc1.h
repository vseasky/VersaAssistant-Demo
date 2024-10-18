// E:\Projects\VersaAssistant-STM32\Desc1.h


char ReportDescriptor[21] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0xb1, 0x00,                    //   FEATURE (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

