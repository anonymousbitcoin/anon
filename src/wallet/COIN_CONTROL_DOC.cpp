// ***************************************************************
//                         DASH
// ***************************************************************
// ***************************************************************
//                         DASH
// ***************************************************************
isminetype mine = IsMine(pcoin->vout[i]);
if (
    // #1 CONDITION
    !(IsSpent(wtxid, i))
    // #2 CONDITION
    && mine != ISMINE_NO 
    // #3 CONDITION
    && (
        // #3.1 CONDITION
        !IsLockedCoin((*it).first, i) 
        // #3.2 CONDITION
        || nCoinType == ONLY_1000) 
    // #4 CONDITION
    && (
        // #4.1 CONDITION
        pcoin->vout[i].nValue > 0 
        // #4.2 CONDITION
        || fIncludeZeroValue) 
    // #5 CONDITION
    && (
        // #5.1 CONDITION
        !coinControl 
        // #5.2 CONDITION
        || !coinControl->HasSelected() 
        // #5.3 CONDITION
        || coinControl->fAllowOtherInputs 
        // #5.4 CONDITION
        || coinControl->IsSelected((*it).first, i))
)
//IF ALL THIS CRAP ABOVE = TRUE DO THIS  
vCoins.push_back(COutput(pcoin, i, nDepth,(
                                          // #1 CONDITION
                                          (mine & ISMINE_SPENDABLE) != ISMINE_NO)
                                          // #2 CONDITION
                                          || (
                                              // #2.1 CONDITION
                                              coinControl &&coinControl->fAllowWatchOnly 
                                              // #2.2 CONDITION
                                              && (mine & ISMINE_WATCH_SOLVABLE) != ISMINE_NO)
                        )
                );
}

// ***************************************************************
//                         BTCP
// ***************************************************************
isminetype mine = IsMine(pcoin->vout[i]);
if (
    // #1 CONDITION
    !(IsSpent(wtxid, i)) 
    // #2 CONDITION
    && mine != ISMINE_NO 
    // #3 CONDITION
    && !IsLockedCoin((*it).first, i) 
    // #4 CONDITION
    && (
        // #4.1 CONDITION
        pcoin->vout[i].nValue > 0 
        // #4.2 CONDITION
        || fIncludeZeroValue) 
    // #5 CONDITION
    && (
        // #5.1 CONDITION
        !coinControl 
        // #5.2 CONDITION
        || !coinControl->HasSelected() 
        // #5.3 CONDITION
        || coinControl->fAllowOtherInputs 
        // #5.4 CONDITION
        || coinControl->IsSelected((*it).first, i))
    )


    //IF ALL THIS CRAP ABOVE = TRUE DO THIS  
    vCoins.push_back(COutput(pcoin, i, nDepth, 
    (mine & ISMINE_SPENDABLE) != ISMINE_NO));
}