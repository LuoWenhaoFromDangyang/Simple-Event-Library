import os
import pathlib
from typing import Callable
SEL_PY: str = 'Unknown'
exec(open(os.path.join(os.path.dirname(os.path.dirname(pathlib.Path(__file__).resolve())), 'common', 'ver.txt')).read())
event_list = dict()
class Events:
    @staticmethod
    def add_event(event: str):
        if not isinstance(event, str):
            raise ValueError("Event must be a string")
        if event not in event_list:
            event_list.update({event: list()})
    @staticmethod
    def remove_event(event: str):
        if not isinstance(event, str):
            raise ValueError("Event must be a string")
        if event in event_list:
            event_list.pop(event)
    @staticmethod
    def get_events():
        return event_list
    @staticmethod
    def clear_events():
        event_list.clear()
    @staticmethod
    def bind(event: str, func: Callable):
        if not isinstance(event, str):
            raise ValueError("Event must be a string")
        if not callable(func):
            raise ValueError("Function must be callable")
        if event not in event_list:
            raise ValueError("Event not found")
        if func not in event_list[event]:
            event_list[event].append(func)
    @staticmethod
    def unbind(event: str, func: Callable):
        if not isinstance(event, str):
            raise ValueError("Event must be a string")
        if not callable(func):
            raise ValueError("Function must be callable")
        if event not in event_list:
            raise ValueError("Event not found")
        if func in event_list[event]:
            event_list[event].remove(func)
    @staticmethod
    def trigger(event: str, *args, **kwargs):
        if not isinstance(event, str):
            raise ValueError("Event must be a string")
        if event not in event_list:
            raise ValueError("Event not found")
        l = []
        for func in event_list[event]:
            l.append(func(*args, **kwargs))
        return l
