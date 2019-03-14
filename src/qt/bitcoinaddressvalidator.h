// Copyright (c) 2011-2014 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef dogxCOIN_QT_dogxCOINADDRESSVALIDATOR_H
#define dogxCOIN_QT_dogxCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class dogxcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit dogxcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** dogxcoin address widget validator, checks for a valid dogxcoin address.
 */
class dogxcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit dogxcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // dogxCOIN_QT_dogxCOINADDRESSVALIDATOR_H
