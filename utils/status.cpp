//
// Created by Dajunnnnnn on 1/13/23.
//
#include "status.h"
namespace db{
    std::string Status::ToString() const {
        if (message1_ == "") {
            return "OK";
        } else {
            char tmp[30];
            const char* type;
            switch (code()) {
                case kOK:
                    type = "OK";
                    break;
                case kNotFound:
                    type = "Not found: ";
                    break;
                case kDeleteOrder:
                    type = "Delete order: ";
                    break;
                case kInvalidArgument:
                    type = "Invalid argument: ";
                    break;
                case kIOError:
                    type = "IO error: ";
                    break;
                case kDone:
                    type = "Done: ";
                    break;
//                case kMultipartRequired:
//                    type = "MultipartRequired: the entry is too large to fit in memory, use the multipart API instead.";
//                    break;
                default:
                    snprintf(tmp, sizeof(tmp), "Unknown code (%d): ",
                             static_cast<int>(code()));
                    type = tmp;
                    break;
            }
            std::string result(type);
            result.append(message1_);
            if (message2_.size() > 0) {
                result.append(" - ");
                result.append(message2_);
            }
            return result;
        }
    }
    const char* Status::CopyState(const char* state) {
        uint32_t size;
        std::memcpy(&size, state, sizeof(size));
        char* result = new char[size + 5];
        std::memcpy(result, state, size + 5);
        return result;
    }
}
