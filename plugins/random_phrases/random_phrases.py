#!/usr/bin/env python
import sys, os, json, time, random, asyncio, aiohttp, async_timeout
from deskchan import PluginInterface


DATA_URL = 'https://sheets.googleapis.com/v4/spreadsheets/17qf7fRewpocQ_TT4FoKWQ3p7gU7gj4nFLbs2mJtBe_k/values/A2:A105?key=AIzaSyDExsxzBLRZgPt1mBKtPCcSDyGgsjM3_uI'


def say_text(text):
    PluginInterface.send_message('dc:say', {'text': text})


@asyncio.coroutine
def say_random_phrase_loop(loop):
    data_dir = (yield from PluginInterface.send_message_and_wait_response('core:query-plugin-data-dir', None))['path']
    #with aiohttp.ClientSession(loop=loop) as session:
    #    with async_timeout.timeout(10):
    #        with session.get(DATA_URL) as response:
    #            pass
    with open('phrases.json', encoding='utf-8') as data_file:
        data = json.load(data_file)
        phrases = [value[0] for value in data['values']]
    del data, data_file
    random.seed()
    while True:
        i = random.randint(0, len(phrases) - 1)
        say_text(phrases[i])
        yield from asyncio.sleep(10)


loop = asyncio.get_event_loop()
PluginInterface.init('random_phrases', loop)
asyncio.ensure_future(say_random_phrase_loop(loop))
loop.run_forever()
loop.close()
