import sys, json, asyncio


def send_json(data):
    print(json.dumps(data))
    sys.stdout.flush()


def init(id):
    send_json({'deskchan-plugin-id': id})
    message_receiver()


def send_message(tag, data, callback=None):
    global response_handlers
    if callback is not None:
        if data is None:
            data = dict()
        assert isinstance(data, dict)
        seq = data.get('seq')
        if seq is None:
            seq = make_seq_value()
            data['seq'] = seq
        response_handlers[seq] = callback
    send_json({'tag': tag, 'data': data})


@asyncio.coroutine
def message_receiver(loop=None):
    global tag_handlers, response_handlers
    if loop is None:
        loop = asyncio.get_event_loop()
    reader = asyncio.StreamReader()
    reader_protocol = asyncio.StreamReaderProtocol(reader)
    yield from loop.connect_read_pipe(lambda: reader_protocol, sys.stdin)
    while True:
        msg_str = yield from reader.readline()
        msg = json.loads(msg_str)
        sender = msg['sender']
        tag = msg['tag']
        data = msg['data']
        handlers = tag_handlers.get(tag, list())
        for handle in handlers:
            handle(sender, tag, data)
        if isinstance(data, dict):
            seq = data.get('seq')
            handler = response_handlers.get(seq)
            if handler is not None:
                handler(sender, tag, data)


def make_seq_value():
    global seq_counter
    seq_counter += 1
    return seq_counter


tag_handlers = dict()
response_handlers = dict()
seq_counter = 0
