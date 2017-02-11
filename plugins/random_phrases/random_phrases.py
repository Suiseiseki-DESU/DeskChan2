#!/usr/bin/env python
import sys, os, json, time, random, asyncio
from deskchan import PluginInterface


DATA_URL = 'https://sheets.googleapis.com/v4/spreadsheets/17qf7fRewpocQ_TT4FoKWQ3p7gU7gj4nFLbs2mJtBe_k/values/A2:A105?key=AIzaSyDExsxzBLRZgPt1mBKtPCcSDyGgsjM3_uI'


def say_text(text):
    PluginInterface.send_message('dc:say', {'text': text})


@asyncio.coroutine
def say_random_phrase_loop(loop):
    data_dir = (yield from PluginInterface.send_message_and_wait_response('core:query-plugin-data-dir', None))['path']
    data_file_name = os.path.join(data_dir, 'phrases.json')
    phrases = None
    try:
        import aiohttp
        response = yield from aiohttp.request('GET', DATA_URL)
        if response.status == 200:
            content = yield from response.read()
            say_text(data_file_name)
            data = json.loads(content)
            phrases = [value[0] for value in data['values']]
            with open(data_file_name, 'w', encoding='utf-8') as data_file:
                data_file.write(json.dumps(data, ensure_ascii=False, indent=4, separators=(', ', ': ')))
    except Exception as e:
        say_text(str(e))
        yield from asyncio.sleep(10)
    if phrases is None:
        with open(data_file_name, encoding='utf-8') as data_file:
            data = json.load(data_file)
            phrases = [value[0] for value in data['values']]
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
