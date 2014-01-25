#ifndef LOGUTIL_H
#define LOGUTIL_H

/**
 * Android-style log utility.
 */
#ifndef NDEBUG
#define LOGD(fmt, ...) qDebug("DEBUG: " __FILE__ ": " fmt, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#endif

#define LOGI(fmt, ...) qDebug("INFO: " __FILE__ ": " fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) qDebug("WARNING: " __FILE__ ": " fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) qDebug("ERROR: " __FILE__ ": " fmt, ##__VA_ARGS__)
#define LOGC(fmt, ...) qDebug("CRITICAL: " __FILE__ ": " fmt, ##__VA_ARGS__)

#endif // LOGUTIL_H
