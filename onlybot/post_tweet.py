import tweepy
import sys
import post_telegram

def tweet(argv):
    """twitter_auth_keys = { 
        "consumer_key"        : "VkMaBepuSCwo4Hbyi92gJXuWE",
        "consumer_secret"     : "OgTxfQ1dlELuEyOkVr3a7RURFiISK98SS1QPcBQdRxUBfitD40",
        "access_token"        : "1852158511-Lz2psBNZIcf6iBXfMcEN4C9wmhm6yijSxQazhEB",
        "access_token_secret" : "1e8aENcPpHV5i8MzOESd2YOwrGBs5vDfEeVHKdYW6aypR"
    }"""

    twitter_auth_keys = { 
        "consumer_key"        : "LfEsVGQNWgf1G2FRIirYsASox",
        "consumer_secret"     : "ipOxZbKV1TZP1h6ZKL1OWb92OCQLEXPQ9Ih6NJ4UaFEimMRJCC",
        "access_token"        : "1410285704060485633-rfsHIg9lEKPh7AqYXH09AF5NtHT0jR",
        "access_token_secret" : "xjlfh5GQJwWxsx62RbLpxAtvZF4n2SYYHsRwZHJfjRmb2"
    }
 
    auth = tweepy.OAuthHandler(
            twitter_auth_keys['consumer_key'],
            twitter_auth_keys['consumer_secret']
            )
    auth.set_access_token(
            twitter_auth_keys['access_token'],
            twitter_auth_keys['access_token_secret']
            )
    api = tweepy.API(auth)
 
    content = argv
    status = api.update_status(status=content)
    """recipient_id = "1025742825512677376"
    Lucas = "1375099327685795844"
    api.send_direct_message(recipient_id, argv)
    api.send_direct_message(Lucas, argv)"""
 
def main(argv):
    tweet(argv)
    post_telegram.send(argv)

if __name__ == "__main__":
    main(sys.argv[1])