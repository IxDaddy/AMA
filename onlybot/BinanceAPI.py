import sys
from binance_f import RequestClient
from binance_f.constant.test import *
from binance_f.base.printobject import *
from binance_f.model.constant import *

def Trade(argv):
    request_client = RequestClient(api_key="jAyLDvqjDaH1tkyEct5ZeQZfXyxeCgCwoZSGS7ttJE1rPext2JP2mFAzUPfFQVpL", secret_key="qWGZ6AR0BHsl6cELHiaGIVQzWNbwjoV8ibsudUK2dq7eXJ6IG4SjKkyzGoxKoD87")
    trade = { 
        "symbole"        : argv[1],
        "qty"            : argv[2],
        "side"           : argv[3],
        "leverage"       : argv[4]
    }
    request_client.change_initial_leverage(trade["symbole"],trade["leverage"])

    if trade["side"] == "1":
        sid = OrderSide.BUY
    elif trade["side"] == "-1":
        sid = OrderSide.SELL
    else:
        return
    request_client.post_order(symbol=str(trade['symbole']), side=sid, ordertype=OrderType.MARKET, quantity=float(trade["qty"]))
    
    
""" format : python3 BiananceAPI <SYMBOL> <TYPE> <QTY> <LEV> """
def main(argv):
    Trade(argv)

if __name__ == "__main__":
    main(sys.argv)