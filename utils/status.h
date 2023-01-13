//
// Created by Dajunnnnnn on 1/13/23.
//

#include <string>

#ifndef KVDB_STATUS_H
#define KVDB_STATUS_H

namespace db{
    class Status{
    public:
        Status(){ code_ = kOK; message1_ = ""; }
        ~Status(){}

        //explicit关键字用来修饰类的构造函数，被修饰的构造函数的类，不能发生相应的隐式类型转换，只能以显示的方式进行类型转换
        //explicit 关键字只能用于类内部的构造函数声明上；explicit 关键字作用于单个参数的构造函数
        explicit Status(int code) : code_(code) {}

        //Right hand side（等号右侧）, left hand side（等号左侧）
        Status(const Status& rhs);
        Status& operator=(const Status& rhs);

        Status(int code, const std::string &message1, const std::string &message2);

        static Status OK() { return Status(); }

        static Status Done() { return Status(kDone); }

        static Status DeleteOrder() { return Status(kDeleteOrder); }

        static Status NotFound(const std::string& message1, const std::string& message2="") {
            return Status(kNotFound, message1, message2);
        }
        static Status InvalidArgument(const std::string &message1, const std::string &message2 = "") {
            return Status(kInvalidArgument, message1, message2);
        }

        static Status IOError(const std::string &message1, const std::string &message2 = "") {
            return Status(kIOError, message1, message2);
        }


        bool isOk() const { return code() == kOK;}
        bool isNotFound() const { return code() == kNotFound;}
        bool isDeleteOrder() const {return code() == kDeleteOrder;}
        bool isInvalidArgument() const {return code() == kInvalidArgument;}
        bool isIOError() const {return code() == kIOError;}
        bool isDone() const {return code() == kDone;}
//        bool isMultipartRequired() const {return code() == kMultipartRequired;}

        std::string ToString() const;



    private:
        int code_;
        std::string message1_;
        std::string message2_;

        int code() const { return code_; };

        enum Code {
            kOK = 0,
            kNotFound = 1,
            kDeleteOrder = 2,
            kInvalidArgument = 3,
            kIOError = 4,
            kDone = 5,
//            kMultipartRequired = 6
        };

        //下面内联函数会用到（leveldb里的）
        static const char* CopyState(const char* s);

        // OK status has a null state_.  Otherwise, state_ is a new[] array
        // of the following form:
        //    state_[0..3] == length of message
        //    state_[4]    == code
        //    state_[5..]  == message
        const char* state_; //leveldb用来标记状态的数组
    };

    //leveldb使用了inline函数来解决构造函数的调用效率
    //引入内联函数的目的是为了解决程序中函数调用的效率问题，这么说吧，程序在编译器编译的时候，编译器将程序中出现的内联函数的调用表达式用内联函数的函数体
    //进行替换，而对于其他的函数，都是在运行时候才被替代。这其实就是个空间代价换时间的i节省。所以内联函数一般都是1-5行的小函数。在使用内联函数时要留神：
    //1.在内联函数内不允许使用循环语句和开关语句；
    //2.内联函数的定义必须出现在内联函数第一次调用之前；
    //3.类结构中所在的类说明内部定义的函数是内联函数。
    inline Status::Status(const Status& rhs) {
        state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    inline Status& Status::operator=(const Status& rhs) {
        // The following condition catches both aliasing (when this == &rhs),
        // and the common case where both rhs and *this are ok.
        if (state_ != rhs.state_) {
            delete[] state_;
            state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
        }
        return *this;
    }

    Status::Status(int code, const std::string &message1, const std::string &message2) :code_(code),message1_(message1),
                                                                                        message2_(message2) {}

} // namespace db

#endif //KVDB_STATUS_H

