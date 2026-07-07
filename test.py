import SEL

def foo(*args):
    a = args[0]
    b = args[1]
    print(f"Received argument: {a:.2f} {b:.2f}")
    return

def main():
    print(f"SEL version: {SEL.SEL_PY}")
    SEL.Events.add_event("foo")
    SEL.Events.bind("foo", foo)
    SEL.Events.trigger("foo", 0.1, 0.2)
    return 0

if __name__ == "__main__":
    main()
