#!/bin/bash
set -e -o pipefail

CURDIR=$(cd $(dirname "$0"); pwd)
# Get BUILDDIR and REAL_BITCOIND
. "${CURDIR}/tests-config.sh"

export BITCOINCLI=${BUILDDIR}/qa/pull-tester/run-bitcoin-cli
export BITCOIND=${REAL_BITCOIND}

#Run the tests

testScripts=(
    # 'prioritisetransaction.py'
    'wallet_treestate.py' #FAILING
    'wallet_protectcoinbase.py' #FAILING
    'wallet_shieldcoinbase.py' #FAILING
    'wallet.py' #FAILING
    'wallet_nullifiers.py' #FAILING
    'wallet_1941.py' #FAILING
    # 'listtransactions.py' #PASSING
    # 'mempool_resurrect_test.py' #PASSES
    # 'txn_doublespend.py' #PASSES
    'txn_doublespend.py --mineblock' #FAILING
    # 'getchaintips.py' #PASSING
    # 'rawtransactions.py' #PASSING
    # 'rest.py'#PASSING
    'mempool_spendcoinbase.py' #FAILING
    'mempool_coinbase_spends.py' #FAILING
    'mempool_tx_input_limit.py' #FAILING
    # 'httpbasics.py' #PASSING
    # 'zapwallettxes.py' #PASSING
    # 'proxy_test.py' #PASSING
    'merkle_blocks.py' #FAILING
    'fundrawtransaction.py' #FAILING
    # 'signrawtransactions.py'#PASSING
    # 'walletbackup.py' #PASSING
    # 'nodehandling.py' #PASSING
    # 'reindex.py' #PASSING
    # 'decodescript.py'#PASSING
    # 'disablewallet.py' #PASSING
    'zcjoinsplit.py' #FAILING
    'zcjoinsplitdoublespend.py' #FAILING
    'getblocktemplate.py' #FAILING
    # 'bip65-cltv-p2p.py'#PASSING
    # 'bipdersig-p2p.py'#PASSING
);
testScriptsExt=(
    'getblocktemplate_longpoll.py'
    'getblocktemplate_proposals.py'
    'pruning.py'
    'forknotify.py'
    'hardforkdetection.py'
    'invalidateblock.py'
    'keypool.py'
    'receivedby.py'
    'rpcbind_test.py'
#   'script_test.py'
    'smartfees.py'
    'maxblocksinflight.py'
    'invalidblockrequest.py'
    'p2p-versionbits-warning.py'
#    'forknotify.py'
    'p2p-acceptblock.py'
);

if [ "x$ENABLE_ZMQ" = "x1" ]; then
  testScripts+=('zmq_test.py') #FAILING
fi

if [ "x$ENABLE_PROTON" = "x1" ]; then
  testScripts+=('proton_test.py')
fi

extArg="-extended"
passOn=${@#$extArg}

successCount=0
declare -a failures

function runTestScript
{
    local testName="$1"
    shift

    echo -e "=== Running testscript ${testName} ==="

    if eval "$@" | sed 's/^/  /'
    then
        successCount=$(expr $successCount + 1)
        echo "--- Success: ${testName} ---"
    else
        failures[${#failures[@]}]="$testName"
        echo "!!! FAIL: ${testName} !!!"
    fi

    echo
}

if [ "x${ENABLE_BITCOIND}${ENABLE_UTILS}${ENABLE_WALLET}" = "x111" ]; then
    for (( i = 0; i < ${#testScripts[@]}; i++ ))
    do
        if [ -z "$1" ] || [ "${1:0:1}" == "-" ] || [ "$1" == "${testScripts[$i]}" ] || [ "$1.py" == "${testScripts[$i]}" ]
        then
            runTestScript \
                "${testScripts[$i]}" \
                "${BUILDDIR}/qa/rpc-tests/${testScripts[$i]}" \
                --srcdir "${BUILDDIR}/src" ${passOn}
        fi
    done
    for (( i = 0; i < ${#testScriptsExt[@]}; i++ ))
    do
        if [ "$1" == $extArg ] || [ "$1" == "${testScriptsExt[$i]}" ] || [ "$1.py" == "${testScriptsExt[$i]}" ]
        then
            runTestScript \
                "${testScriptsExt[$i]}" \
                "${BUILDDIR}/qa/rpc-tests/${testScriptsExt[$i]}" \
                --srcdir "${BUILDDIR}/src" ${passOn}
        fi
    done

    echo -e "\n\nTests completed: $(expr $successCount + ${#failures[@]})"
    echo "successes $successCount; failures: ${#failures[@]}"

    if [ ${#failures[@]} -gt 0 ]
    then
        echo -e "\nFailing tests: ${failures[*]}"
        exit 1
    else
        exit 0
    fi
else
  echo "No rpc tests to run. Wallet, utils, and bitcoind must all be enabled"
fi
