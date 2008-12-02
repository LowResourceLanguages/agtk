%header %{
#define raise_error(msg) { \
        string message = "WrapperError\x01"; \
        message += msg; \
        SWIG_exception(SWIG_RuntimeError, (char*) message.c_str()); \
    }
%}
