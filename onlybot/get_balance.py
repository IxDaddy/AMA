from binance_f import RequestClient
from binance_f.constant.test import *
from binance_f.base.printobject import *
from binance_f.model.constant import *
from post_tweet import *

request_client = RequestClient(api_key="jAyLDvqjDaH1tkyEct5ZeQZfXyxeCgCwoZSGS7ttJE1rPext2JP2mFAzUPfFQVpL", secret_key="qWGZ6AR0BHsl6cELHiaGIVQzWNbwjoV8ibsudUK2dq7eXJ6IG4SjKkyzGoxKoD87")
result = request_client.get_balance_v2()
tweet(result["balance"])

