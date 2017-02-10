#!/usr/bin/env python
import sys, os, json, time, random


def print_json(data):
    print(json.dumps(data))
    sys.stdout.flush()


def initialize_plugin_interface(id):
    print_json({'deskchan-plugin-id': id})


def send_message(tag, data):
    print_json({'tag': tag, 'data': data})


def say_text(text):
    send_message('dc:say', {'text': text})


initialize_plugin_interface('random_phrases')


with open('phrases.json') as data_file:
    data = json.load(data_file)
phrases = [value[0] for value in data['values']]
del data, data_file


random.seed()


while True:
    i = random.randint(0, len(phrases) - 1)
    say_text(phrases[i])
    time.sleep(10)
