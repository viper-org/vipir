f main(argc) {
    x = 5
    y = &x
    x = 12
    call test(1, 2, 3, 4, 5, 6, 7, 8)
    return x
}

f test {
    return
}