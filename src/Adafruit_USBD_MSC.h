/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef ADAFRUIT_USBD_MSC_H_
#define ADAFRUIT_USBD_MSC_H_

#include "Adafruit_TinyUSB_Core.h"

class Adafruit_USBD_MSC : Adafruit_USBD_Interface
{
  public:
    typedef int32_t (*read_callback_t ) (uint32_t lba, void* buffer, uint32_t bufsize);
    typedef int32_t (*write_callback_t) (uint32_t lba, uint8_t* buffer, uint32_t bufsize);
    typedef void    (*flush_callback_t) (void);

    Adafruit_USBD_MSC(void);

    bool begin(void);

    void setMaxLun(uint8_t maxlun);
    uint8_t getMaxLun(void);

    void setID(uint8_t lun, const char* vendor_id, const char* product_id, const char* product_rev);

    void setCapacity(uint8_t lun, uint32_t block_count, uint16_t block_size);
    void getCapacity(uint8_t lun, uint32_t* block_count, uint16_t* block_size);

    void setCallback(uint8_t lun, read_callback_t rd_cb, write_callback_t wr_cb, flush_callback_t fl_cb);

    // from Adafruit_USBD_Interface
    virtual uint16_t getDescriptor(uint8_t* buf, uint16_t bufsize);

  private:
    enum { MAX_LUN = 2 };
    struct {
        read_callback_t  rd_cb;
        write_callback_t wr_cb;
        flush_callback_t fl_cb;

        uint32_t block_count;
        uint16_t block_size;

        const char* _inquiry_vid;
        const char* _inquiry_pid;
        const char* _inquiry_rev;
    } _lun[MAX_LUN];

    uint8_t _maxlun;

    friend int32_t tud_msc_read10_cb (uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    friend int32_t tud_msc_write10_cb (uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
    friend void tud_msc_write10_complete_cb (uint8_t lun);
    friend void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);
};

#endif /* ADAFRUIT_USBD_MSC_H_ */
