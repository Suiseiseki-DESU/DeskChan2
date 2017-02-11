import os, sys, json, asyncio


def send_json(data):
    print(json.dumps(data))
    sys.stdout.flush()


def init(id, loop):
    send_json({'deskchan-plugin-id': id})
    asyncio.ensure_future(message_receiver(loop))


def subscribe(tag, callback=None):
    global tag_handlers
    if callback is not None:
        l = tag_handlers.get(tag)
        if l is None:
            l = list()
            tag_handlers[tag] = l
        l.append(callback)
    send_json({'subscribe': tag})


def unsubscribe(tag):
    global tag_handlers
    send_json({'unsubscribe': tag})
    del tag_handlers[tag]


def send_message(tag, data=None, callback=None):
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


def send_message_and_wait_response(tag, data=None):
    f = asyncio.Future()
    def done(sender, tag, data):
        f.set_result(data)
    send_message(tag, data, done)
    return f


@asyncio.coroutine
def message_receiver(loop=None):
    global tag_handlers, response_handlers, IS_WINDOWS
    if loop is None:
        loop = asyncio.get_event_loop()
    if not IS_WINDOWS:
        reader = asyncio.StreamReader()
        reader_protocol = asyncio.StreamReaderProtocol(reader)
        yield from loop.connect_read_pipe(lambda: reader_protocol, sys.stdin)
    while True:
        if not IS_WINDOWS:
            msg_str = yield from reader.readline()
        else:
            msg_str = yield from loop.run_in_executor(None, sys.stdin.readline)
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
                del response_handlers[seq]


def make_seq_value():
    global seq_counter
    seq_counter += 1
    return seq_counter


IS_WINDOWS = os.name == 'nt'
tag_handlers = dict()
response_handlers = dict()
seq_counter = 0
