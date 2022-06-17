/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 11:49:47
 * LastEditTime : 2022-06-17 12:24:35
 * Description  : 
 */

#include "userio.h"
#include "userdisplay.h"
#include "str.h"

namespace userlib {
    int printf_add_to_buffer(char *buffer, char c, int &idx, const int BUF_LEN)
    {
        int counter = 0;

        buffer[idx] = c;
        ++idx;

        if (idx == BUF_LEN)
        {
            buffer[idx] = '\0';
            counter = print(buffer);
            idx = 0;
        }

        return counter;
    }

    int printf(const char* format, ...) {
        const int BUF_LEN = 32;

        char buffer[BUF_LEN + 1];
        char number[33];

        int idx, counter;
        va_list ap;

        va_start(ap, format);
        idx = 0;
        counter = 0;

        for (int i = 0; format[i]; ++i)
        {
            if (format[i] != '%')
            {
                counter += printf_add_to_buffer(buffer, format[i], idx, BUF_LEN);
            }
            else
            {
                i++;
                if (format[i] == '\0')
                {
                    break;
                }

                switch (format[i])
                {
                case '%':
                    counter += printf_add_to_buffer(buffer, format[i], idx, BUF_LEN);
                    break;

                case 'c':
                    counter += printf_add_to_buffer(buffer, va_arg(ap, char), idx, BUF_LEN);
                    break;

                case 's':
                    buffer[idx] = '\0';
                    idx = 0;
                    counter += print(buffer);
                    counter += print(va_arg(ap, const char *));
                    break;

                case 'd':
                case 'x':
                    int temp = va_arg(ap, int);

                    if (temp < 0 && format[i] == 'd')
                    {
                        counter += printf_add_to_buffer(buffer, '-', idx, BUF_LEN);
                        temp = -temp;
                    }

                    itos(number, temp, (format[i] == 'd' ? 10 : 16));

                    for (int j = 0; number[j]; ++j)
                    {
                        counter += printf_add_to_buffer(buffer, number[j], idx, BUF_LEN);
                    }
                    break;
                }
            }
        }

        buffer[idx] = '\0';
        counter += print(buffer);

        return counter;
    }
}