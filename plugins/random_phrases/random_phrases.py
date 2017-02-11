#!/usr/bin/env python
import sys, os, json, time, random
from deskchan import PluginInterface


def say_text(text):
    PluginInterface.send_message('dc:say', {'text': text})


PluginInterface.init('random_phrases')


with open('phrases.json', encoding='utf-8') as data_file:
    data = json.load(data_file)
phrases = [value[0] for value in data['values']]
del data, data_file


random.seed()


while True:
    i = random.randint(0, len(phrases) - 1)
    say_text(phrases[i])
    time.sleep(10)
