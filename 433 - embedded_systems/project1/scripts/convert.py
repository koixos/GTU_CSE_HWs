import cv2

while True:
    filename = input()
    img = cv2.imread(("./imgs/" + filename + ".jpeg"), cv2.IMREAD_GRAYSCALE)
    img = cv2.resize(img, (64, 64), interpolation=cv2.INTER_AREA)
    cv2.imwrite(("./convs/" + filename + ".pgm"), img)