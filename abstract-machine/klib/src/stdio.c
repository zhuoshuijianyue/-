#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
int sprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* buffer_ptr = buffer; // Pointer to the current position in the buffer

    for (const char* fmt_ptr = format; *fmt_ptr; ++fmt_ptr) {
        if (*fmt_ptr != '%') {
            // Copy regular characters to the buffer
            *buffer_ptr++ = *fmt_ptr;
        } else {
            // Handle format specifiers
            ++fmt_ptr; // Move past '%'

            // Check for width specifier like %02d or %04d
            int width = 0;
            while (*fmt_ptr >= '0' && *fmt_ptr <= '9') {
                width = width * 10 + (*fmt_ptr - '0');
                ++fmt_ptr;
            }

            // Check for the format character (e.g., 'd' for %02d, 's' for %s)
            if (*fmt_ptr == 'd') {
                int value = va_arg(args, int);

                // Format the integer and copy it to the buffer
                int num_digits = 0;
                int temp_value = value;

                // Calculate the number of digits in the integer
                do {
                    temp_value /= 10;
                    num_digits++;
                } while (temp_value != 0);

                // Add leading zeros if necessary
                while (num_digits < width) {
                    *buffer_ptr++ = '0';
                    num_digits++;
                }

                // Copy the integer to the buffer
                char temp[12]; // Assuming a maximum width of 11 characters for an int
                char* temp_ptr = temp + 11;
                *temp_ptr = '\0';

                if (value == 0) {
                    *(--temp_ptr) = '0';
                } else {
                    while (value != 0) {
                        *(--temp_ptr) = '0' + (value % 10);
                        value /= 10;
                    }
                }

                while (*temp_ptr) {
                    *buffer_ptr++ = *temp_ptr++;
                }
            } else if (*fmt_ptr == 's') {
                // Handle %s format specifier (string)
                const char* str = va_arg(args, const char*);

                // Copy the string to the buffer
                while (*str) {
                    *buffer_ptr++ = *str++;
                }
            } else {
                // Unsupported format specifier, just copy it
                *buffer_ptr++ = '%';
                if (width > 0) {
                    // Copy width specifier if present
                    char width_str[12]; // Assuming a maximum width of 11 characters
                    char* width_str_ptr = width_str + 11;
                    *width_str_ptr = '\0';

                    int width_copy = width;
                    int width_num_digits = 0;

                    if (width_copy == 0) {
                        *(--width_str_ptr) = '0';
                        width_num_digits = 1;
                    } else {
                        while (width_copy != 0) {
                            *(--width_str_ptr) = '0' + (width_copy % 10);
                            width_copy /= 10;
                            width_num_digits++;
                        }
                    }

                    while (width_num_digits < width) {
                        *buffer_ptr++ = '0';
                        width_num_digits++;
                    }

                    while (*width_str_ptr) {
                        *buffer_ptr++ = *width_str_ptr++;
                    }
                }
                *buffer_ptr++ = *fmt_ptr; // Copy the format character
            }
        }
    }

    *buffer_ptr = '\0'; // Null-terminate the buffer
    
    va_end(args);
    if(buffer==NULL) return -1;
    else return strlen(buffer);
}



int printf(const char *format, ...) {
    char buffer[4095];
    va_list args;
    va_start(args, format);

    char* buffer_ptr = buffer; // Pointer to the current position in the buffer

    for (const char* fmt_ptr = format; *fmt_ptr; ++fmt_ptr) {
        if (*fmt_ptr != '%') {
            // Copy regular characters to the buffer
            *buffer_ptr++ = *fmt_ptr;
        } else {
            // Handle format specifiers
            ++fmt_ptr; // Move past '%'

            // Check for width specifier like %02d or %04d
            int width = 0;
            while (*fmt_ptr >= '0' && *fmt_ptr <= '9') {
                width = width * 10 + (*fmt_ptr - '0');
                ++fmt_ptr;
            }

            // Check for the format character (e.g., 'd' for %02d, 's' for %s)
            if (*fmt_ptr == 'd') {
                int value = va_arg(args, int);

                // Format the integer and copy it to the buffer
                int num_digits = 0;
                int temp_value = value;

                // Calculate the number of digits in the integer
                do {
                    temp_value /= 10;
                    num_digits++;
                } while (temp_value != 0);

                // Add leading zeros if necessary
                while (num_digits < width) {
                    *buffer_ptr++ = '0';
                    num_digits++;
                }

                // Copy the integer to the buffer
                char temp[12]; // Assuming a maximum width of 11 characters for an int
                char* temp_ptr = temp + 11;
                *temp_ptr = '\0';

                if (value == 0) {
                    *(--temp_ptr) = '0';
                } else {
                    while (value != 0) {
                        *(--temp_ptr) = '0' + (value % 10);
                        value /= 10;
                    }
                }

                while (*temp_ptr) {
                    *buffer_ptr++ = *temp_ptr++;
                }
            } else if (*fmt_ptr == 's') {
                // Handle %s format specifier (string)
                const char* str = va_arg(args, const char*);

                // Copy the string to the buffer
                while (*str) {
                    *buffer_ptr++ = *str++;
                }
            } else {
                // Unsupported format specifier, just copy it
                *buffer_ptr++ = '%';
                if (width > 0) {
                    // Copy width specifier if present
                    char width_str[12]; // Assuming a maximum width of 11 characters
                    char* width_str_ptr = width_str + 11;
                    *width_str_ptr = '\0';

                    int width_copy = width;
                    int width_num_digits = 0;

                    if (width_copy == 0) {
                        *(--width_str_ptr) = '0';
                        width_num_digits = 1;
                    } else {
                        while (width_copy != 0) {
                            *(--width_str_ptr) = '0' + (width_copy % 10);
                            width_copy /= 10;
                            width_num_digits++;
                        }
                    }

                    while (width_num_digits < width) {
                        *buffer_ptr++ = '0';
                        width_num_digits++;
                    }

                    while (*width_str_ptr) {
                        *buffer_ptr++ = *width_str_ptr++;
                    }
                }
                *buffer_ptr++ = *fmt_ptr; // Copy the format character
            }
        }
    }

    *buffer_ptr = '\0'; // Null-terminate the buffer
    
    va_end(args);
    int num_temp=0;
    if(buffer==NULL) return -1;
    else num_temp=strlen(buffer);

    for(int g=0;*(buffer+g)!='\0';g++)
    putch(*(buffer+g));
     
    return num_temp;
}




int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}




#endif
