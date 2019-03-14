#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

dogxCOIND=${dogxCOIND:-$BINDIR/dogxcoind}
dogxCOINCLI=${dogxCOINCLI:-$BINDIR/dogxcoin-cli}
dogxCOINTX=${dogxCOINTX:-$BINDIR/dogxcoin-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/dogxcoin-wallet}
dogxCOINQT=${dogxCOINQT:-$BINDIR/qt/dogxcoin-qt}

[ ! -x $dogxCOIND ] && echo "$dogxCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little dogx
BTCVER=($($dogxCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for dogxcoind if --version-string is not set,
# but has different outcomes for dogxcoin-qt and dogxcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$dogxCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $dogxCOIND $dogxCOINCLI $dogxCOINTX $WALLET_TOOL $dogxCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
