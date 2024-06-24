//
// Created by kali on 6/23/24.
//

#ifndef SERVER_H
#define SERVER_H

#include <string>
enum STATUS_CODE
{
    // Informational responses
    CONTINUE                        = 100,
    SWITCHING_PROTOCOLS             = 101,
    PROCESSING                      = 102,
    EARLY_HINTS                     = 103,

    // Successful responses
    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORITATIVE_INFORMATION   = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,
    MULTI_STATUS                    = 207,
    ALREADY_REPORTED                = 208,
    IM_USED                         = 226,

    // Redirection messages
    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,  // Deprecated
    UNUSED                          = 306,  // No longer used
    TEMPORARY_REDIRECT              = 307,
    PERMANENT_REDIRECT              = 308,

    // Client error responses
    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,  // Experimental
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406,
    PROXY_AUTHENTICATION_REQUIRED   = 407,
    REQUEST_TIMEOUT                 = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    PAYLOAD_TOO_LARGE               = 413,
    URI_TOO_LONG                    = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    RANGE_NOT_SATISFIABLE           = 416,
    EXPECTATION_FAILED              = 417,
    IM_A_TEAPOT                     = 418,
    MISDIRECTED_REQUEST             = 421,
    UNPROCESSABLE_CONTENT           = 422,
    LOCKED                          = 423,
    FAILED_DEPENDENCY               = 424,
    TOO_EARLY                       = 425,  // Experimental
    UPGRADE_REQUIRED                = 426,
    PRECONDITION_REQUIRED           = 428,
    TOO_MANY_REQUESTS               = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS   = 451,

    // Server error responses
    INTERNAL_SERVER_ERROR           = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVICE_UNAVAILABLE             = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,
    VARIANT_ALSO_NEGOTIATES         = 506,
    INSUFFICIENT_STORAGE            = 507,
    LOOP_DETECTED                   = 508,
    NOT_EXTENDED                    = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
};

constexpr std::string status_code_to_string(const STATUS_CODE code) {
    switch(code) {
        case CONTINUE:                        return "CONTINUE";
        case SWITCHING_PROTOCOLS:             return "SWITCHING_PROTOCOLS";
        case PROCESSING:                      return "PROCESSING";
        case EARLY_HINTS:                     return "EARLY_HINTS";
        case OK:                              return "OK";
        case CREATED:                         return "CREATED";
        case ACCEPTED:                        return "ACCEPTED";
        case NON_AUTHORITATIVE_INFORMATION:   return "NON_AUTHORITATIVE_INFORMATION";
        case NO_CONTENT:                      return "NO_CONTENT";
        case RESET_CONTENT:                   return "RESET_CONTENT";
        case PARTIAL_CONTENT:                 return "PARTIAL_CONTENT";
        case MULTI_STATUS:                    return "MULTI_STATUS";
        case ALREADY_REPORTED:                return "ALREADY_REPORTED";
        case IM_USED:                         return "IM_USED";
        case MULTIPLE_CHOICES:                return "MULTIPLE_CHOICES";
        case MOVED_PERMANENTLY:               return "MOVED_PERMANENTLY";
        case FOUND:                           return "FOUND";
        case SEE_OTHER:                       return "SEE_OTHER";
        case NOT_MODIFIED:                    return "NOT_MODIFIED";
        case USE_PROXY:                       return "USE_PROXY";  // Deprecated
        case UNUSED:                          return "UNUSED";  // No longer used
        case TEMPORARY_REDIRECT:              return "TEMPORARY_REDIRECT";
        case PERMANENT_REDIRECT:              return "PERMANENT_REDIRECT";
        case BAD_REQUEST:                     return "BAD_REQUEST";
        case UNAUTHORIZED:                    return "UNAUTHORIZED";
        case PAYMENT_REQUIRED:                return "PAYMENT_REQUIRED";  // Experimental
        case FORBIDDEN:                       return "FORBIDDEN";
        case NOT_FOUND:                       return "NOT_FOUND";
        case METHOD_NOT_ALLOWED:              return "METHOD_NOT_ALLOWED";
        case NOT_ACCEPTABLE:                  return "NOT_ACCEPTABLE";
        case PROXY_AUTHENTICATION_REQUIRED:   return "PROXY_AUTHENTICATION_REQUIRED";
        case REQUEST_TIMEOUT:                 return "REQUEST_TIMEOUT";
        case CONFLICT:                        return "CONFLICT";
        case GONE:                            return "GONE";
        case LENGTH_REQUIRED:                 return "LENGTH_REQUIRED";
        case PRECONDITION_FAILED:             return "PRECONDITION_FAILED";
        case PAYLOAD_TOO_LARGE:               return "PAYLOAD_TOO_LARGE";
        case URI_TOO_LONG:                    return "URI_TOO_LONG";
        case UNSUPPORTED_MEDIA_TYPE:          return "UNSUPPORTED_MEDIA_TYPE";
        case RANGE_NOT_SATISFIABLE:           return "RANGE_NOT_SATISFIABLE";
        case EXPECTATION_FAILED:              return "EXPECTATION_FAILED";
        case IM_A_TEAPOT:                     return "IM_A_TEAPOT";
        case MISDIRECTED_REQUEST:             return "MISDIRECTED_REQUEST";
        case UNPROCESSABLE_CONTENT:           return "UNPROCESSABLE_CONTENT";
        case LOCKED:                          return "LOCKED";
        case FAILED_DEPENDENCY:               return "FAILED_DEPENDENCY";
        case TOO_EARLY:                       return "TOO_EARLY";  // Experimental
        case UPGRADE_REQUIRED:                return "UPGRADE_REQUIRED";
        case PRECONDITION_REQUIRED:           return "PRECONDITION_REQUIRED";
        case TOO_MANY_REQUESTS:               return "TOO_MANY_REQUESTS";
        case REQUEST_HEADER_FIELDS_TOO_LARGE: return "REQUEST_HEADER_FIELDS_TOO_LARGE";
        case UNAVAILABLE_FOR_LEGAL_REASONS:   return "UNAVAILABLE_FOR_LEGAL_REASONS";
        case INTERNAL_SERVER_ERROR:           return "INTERNAL_SERVER_ERROR";
        case NOT_IMPLEMENTED:                 return "NOT_IMPLEMENTED";
        case BAD_GATEWAY:                     return "BAD_GATEWAY";
        case SERVICE_UNAVAILABLE:             return "SERVICE_UNAVAILABLE";
        case GATEWAY_TIMEOUT:                 return "GATEWAY_TIMEOUT";
        case HTTP_VERSION_NOT_SUPPORTED:      return "HTTP_VERSION_NOT_SUPPORTED";
        case VARIANT_ALSO_NEGOTIATES:         return "VARIANT_ALSO_NEGOTIATES";
        case INSUFFICIENT_STORAGE:            return "INSUFFICIENT_STORAGE";
        case LOOP_DETECTED:                   return "LOOP_DETECTED";
        case NOT_EXTENDED:                    return "NOT_EXTENDED";
        case NETWORK_AUTHENTICATION_REQUIRED: return "NETWORK_AUTHENTICATION_REQUIRED";
        default:                              return "UNKNOWN_STATUS_CODE";
    }
}

inline std::string format_status_string(const std::string& str) {
    std::string result;
    bool capitalize_next = true;

    for (char ch : str) {
        if (ch == '_') {
            result += ' ';
            capitalize_next = true;
        } else {
            if (capitalize_next) {
                result += std::toupper(ch);
                capitalize_next = false;
            } else {
                result += std::tolower(ch);
            }
        }
    }

    return result;
}
#endif //SERVER_H
