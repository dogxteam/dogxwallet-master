#ifndef dogxCOIN_QT_TEST_UTIL_H
#define dogxCOIN_QT_TEST_UTIL_H

#include <QString>

/**
 * Press "Ok" button in message box dialog.
 *
 * @param text - Optionally store dialog text.
 * @param msec - Number of milliseconds to pause before triggering the callback.
 */
void ConfirmMessage(QString* text = nullptr, int msec = 0);

#endif // dogxCOIN_QT_TEST_UTIL_H
