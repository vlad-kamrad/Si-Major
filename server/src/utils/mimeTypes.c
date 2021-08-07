#include <stddef.h>
#include <string.h>
#include "utils.h"
#include "../collections/dictionary.h"

struct Dictionary MIME_Types;

static void init_MIME_Types()
{
    MIME_Types = new_Dictionary();

    dictAdd(&MIME_Types, "aac", "audio/aac");
    dictAdd(&MIME_Types, "abw", "application/x-abiword");
    dictAdd(&MIME_Types, "arc", "application/x-freearc");
    dictAdd(&MIME_Types, "avi", "video/x-msvideo");
    dictAdd(&MIME_Types, "azw", "application/vnd.amazon.ebook");
    dictAdd(&MIME_Types, "bin", "application/octet-stream");
    dictAdd(&MIME_Types, "bmp", "image/bmp");
    dictAdd(&MIME_Types, "bz", "application/x-bzip");
    dictAdd(&MIME_Types, "bz2", "application/x-bzip2");
    dictAdd(&MIME_Types, "csh", "application/x-csh");
    dictAdd(&MIME_Types, "css", "text/css");
    dictAdd(&MIME_Types, "csv", "text/csv");
    dictAdd(&MIME_Types, "doc", "application/msword");
    dictAdd(&MIME_Types, "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    dictAdd(&MIME_Types, "eot", "application/vnd.ms-fontobject");
    dictAdd(&MIME_Types, "epub", "application/epub+zip");
    dictAdd(&MIME_Types, "gz", "application/gzip");
    dictAdd(&MIME_Types, "gif", "image/gif");
    dictAdd(&MIME_Types, "htm", "text/html");
    dictAdd(&MIME_Types, "html", "text/html");
    dictAdd(&MIME_Types, "ico", "image/x-icon");
    dictAdd(&MIME_Types, "ics", "text/calendar");
    dictAdd(&MIME_Types, "jar", "application/java-archive");
    dictAdd(&MIME_Types, "jpeg", "image/jpeg");
    dictAdd(&MIME_Types, "jpg", "image/jpeg");
    dictAdd(&MIME_Types, "js", "text/javascript");
    dictAdd(&MIME_Types, "json", "application/json");
    dictAdd(&MIME_Types, "jsonld", "application/ld+json");
    dictAdd(&MIME_Types, "mjs", "text/javascript");
    dictAdd(&MIME_Types, "mp3", "audio/mpeg");
    dictAdd(&MIME_Types, "mpeg", "video/mpeg");
    dictAdd(&MIME_Types, "mpkg", "application/vnd.apple.installer+xml");
    dictAdd(&MIME_Types, "odp", "application/vnd.oasis.opendocument.presentation");
    dictAdd(&MIME_Types, "ods", "application/vnd.oasis.opendocument.spreadsheet");
    dictAdd(&MIME_Types, "odt", "application/vnd.oasis.opendocument.text");
    dictAdd(&MIME_Types, "oga", "audio/ogg");
    dictAdd(&MIME_Types, ".ogv", "video/ogg");
    dictAdd(&MIME_Types, "ogx", "application/ogg");
    dictAdd(&MIME_Types, "opus", "audio/opus");
    dictAdd(&MIME_Types, "otf", "font/otf");
    dictAdd(&MIME_Types, "png", "image/png");
    dictAdd(&MIME_Types, "pdf", "application/pdf");
    dictAdd(&MIME_Types, "php", "application/php");
    dictAdd(&MIME_Types, "ppt", "application/vnd.ms-powerpoint");
    dictAdd(&MIME_Types, "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
    dictAdd(&MIME_Types, "rar", "application/vnd.rar");
    dictAdd(&MIME_Types, "rtf", "application/rtf");
    dictAdd(&MIME_Types, "sh", "application/x-sh");
    dictAdd(&MIME_Types, "svg", "image/svg+xml");
    dictAdd(&MIME_Types, "swf", "application/x-shockwave-flash");
    dictAdd(&MIME_Types, "tar", "application/x-tar");
    dictAdd(&MIME_Types, "tif", "image/tiff");
    dictAdd(&MIME_Types, "tiff", "image/tiff");
    dictAdd(&MIME_Types, "ts", "video/mp2t");
    dictAdd(&MIME_Types, "ttf", "font/ttf");
    dictAdd(&MIME_Types, "txt", "text/plain");
    dictAdd(&MIME_Types, "vsd", "application/vnd.visio");
    dictAdd(&MIME_Types, "wav", "audio/wav");
    dictAdd(&MIME_Types, "weba", "audio/webm");
    dictAdd(&MIME_Types, "webm", "video/webm");
    dictAdd(&MIME_Types, "webp", "image/webp");
    dictAdd(&MIME_Types, "woff", "font/woff");
    dictAdd(&MIME_Types, "woff2", "font/woff2");
    dictAdd(&MIME_Types, "xhtml", "application/xhtml+xml");
    dictAdd(&MIME_Types, "xls", "application/vnd.ms-excel");
    dictAdd(&MIME_Types, "xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    dictAdd(&MIME_Types, "xml", "XML");
    dictAdd(&MIME_Types, "xul", "application/vnd.mozilla.xul+xml");
    dictAdd(&MIME_Types, "zip", "application/zip");
    dictAdd(&MIME_Types, "3gp", "video/3gpp");
    dictAdd(&MIME_Types, "3g2", "video/3gpp2");
    dictAdd(&MIME_Types, "7z", "application/x-7z-compressed");
}

char *get_MIME_Type(char *extension)
{
    if (!MIME_Types.count)
    {
        init_MIME_Types();
    }

    return dictGet(&MIME_Types, extension);
}
