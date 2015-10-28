#ifndef SENSOR_HUB_HPP
#define SENSOR_HUB_HPP

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <map>
#include <limits>
#include <memory>
#include <errno.h>
#include <endian.h>
#include <type_traits>
#include <memory>
#include <algorithm>

#include "Endian.hpp"

/** The register number (2 bytes), and length (2 bytes). */
#define SENSORHUB_CMD_LENGTH 4
#define SH_MAX_GENERIC_DATA 512

#if defined(MOTOSH) || defined(MODULE_motosh)
    #include "linux/motosh.h"
    #define SH_DRIVER "/dev/motosh"
    #define SH_IOCTL_READ_REG   MOTOSH_IOCTL_READ_REG
    #define SH_IOCTL_WRITE_REG  MOTOSH_IOCTL_WRITE_REG
    #define SH_IOCTL_GET_VERNAME  MOTOSH_IOCTL_GET_VERNAME
#elif defined(STML0XX) || defined(MODULE_stml0xx)
    #include "linux/stml0xx.h"
    #define SH_DRIVER "/dev/stml0xx"
    #define SH_IOCTL_READ_REG   STML0XX_IOCTL_READ_REG
    #define SH_IOCTL_WRITE_REG  STML0XX_IOCTL_WRITE_REG
    #define SH_IOCTL_GET_VERNAME STML0XX_IOCTL_GET_VERNAME
#endif

namespace mot {

class SensorHub {
    private:
        int fd;

        /** SensorHub is assumed to be little endian. In the future this should
         * be auto-detected. */
        static const bool isBigEndian = false;

    public:

        #define VMM_ENTRY(reg, id, writable, addr, size) id,
        enum struct VmmID : uint16_t {
            #include "linux/motosh_vmm.h"
        };
        #undef VMM_ENTRY

        static const std::map<std::string, uint16_t> Vmm;

        SensorHub() {
            fd = open(SH_DRIVER, O_RDONLY|O_WRONLY);
        }

        ~SensorHub() {
            close(fd);
        }

        /** Convert from SensorHub endianess to host endianess (or vice versa).
         */
        template<typename T> static inline T EndianCvt(T val) {
            return isBigEndian == Endian::isBigEndian() ? val : Endian::swap<T>(val);
        }

        /** A wrapper around the ioctl() call that retries if it was interrupted. */
        static int retryIoctl (int fd, int ioctl_number, ...);

        /** Looks up the SensorHub register number given its name.
         *
         * @param regName The register name. See the
         * kernel/include/uapi/linux/motosh_vmm.h file for a list of valid names.
         *
         * @return The register number, or a negative value on error.
         */
        int16_t getRegisterNumber(std::string regName);

        /** Reads the contents of a SensorHub register.
         *
         * @param regNr The register number to read from.
         * @param size The number of bytes to read.
         *
         * @return An array of bytes with the register contents, or a null pointer if
         * an error was encountered.
         */
        std::unique_ptr<uint8_t[]> readReg(VmmID vmmId, uint16_t size);

        /** Reads the contents of a SensorHub register.
         *
         * @param regName The register name. See the
         * kernel/include/uapi/linux/motosh_vmm.h file for a list of valid names.
         * @param size The number of bytes to read.
         *
         * @return An array of bytes with the register contents, or a null pointer if
         * an error was encountered.
         */
        std::unique_ptr<uint8_t[]> readReg(const std::string regName, uint16_t size);

        /** Write a block of data to the SensorHub
         *
         * @param data The data to write. The caller is responsible for adding
         * the proper header.
         *
         * @return The IOCTL result.
         */
        inline int writeReg(std::unique_ptr<uint8_t[]> const & data) const {
            return retryIoctl(fd, SH_IOCTL_WRITE_REG, data.get());
        }

        /** Write a block of data to a specific SensorHub register after adding
         * the appropriate buffer headers.
         *
         * @param regName The register name. See the
         * kernel/include/uapi/linux/motosh_vmm.h file for a list of valid names.
         * @param size The number of bytes to write.
         * @param data The data to write. This should not include any
         * register/size headers.
         */
        bool writeReg(const std::string regName, uint16_t size,
                const uint8_t * const data);

        /**
         * Wrapper around the IOCTL_GET_VERNAME ioctl() call.
         *
         * @return On success, the version name. On error, an empty string.
         */
        std::string getVariant(void);

        /** Reads the version string from the SensorHub.
         *
         * @return On success, the version string. On error, an empty string.
         */
        std::string getVersionStr(void);

        /** Reads the flash CRC computed by the SensorHub.
         *
         * @return The CRC, or 0 on error (assuming the real CRC will never by
         * exactly 0).
         */
        uint32_t getFlashCrc(void);
};

} // namespace mot

#endif // SENSOR_HUB_HPP