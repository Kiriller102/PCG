from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt5.QtGui import QImage, QPainter

import sys
import time
import pygame as pg

WEIGHT = 600
HEIGHT = 600
MID = (WEIGHT / 2, HEIGHT / 2)
DARKGREY = (70, 70, 70)
WHITE = (200, 200, 200)
GRAY = (100, 100, 100)
BLUE = (70, 70, 130)
BLACK = (0, 0, 0)
OXY = (40, 40, 40)
step = 10
x = WEIGHT / 2
y = HEIGHT / 2


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setWindowTitle("menu")
        self.setFixedSize(140, 210)
        pbc = QtWidgets.QPushButton("Circle", self)
        pbc.move(20, 10)
        pbc.setFixedSize(100, 30)
        pbc.clicked.connect(self.CirclePB)
        pbl = QtWidgets.QPushButton("Line", self)
        pbl.move(20, 50)
        pbl.setFixedSize(100, 30)
        pbl.clicked.connect(self.StepLine)
        pbb = QtWidgets.QPushButton("line BR", self)
        pbb.move(20, 90)
        pbb.setFixedSize(100, 30)
        pbb.clicked.connect(self.BresenhamLine)
        pbc = QtWidgets.QPushButton("line DDA", self)
        pbc.move(20, 130)
        pbc.setFixedSize(100, 30)
        pbc.clicked.connect(self.CDALine)
        pbs = QtWidgets.QPushButton("smoothing", self)
        pbs.move(20, 170)
        pbs.setFixedSize(100, 30)
        pbs.clicked.connect(self.SmoothingLine)

    def CirclePB(self):
        self.hide()
        le = QtWidgets.QLineEdit()
        R, ok = QtWidgets.QInputDialog.getInt(self, 'Input R',
                                              'Enter R: ')

        if ok:
            DrawCircle(R * 10)
        self.show()

    def StepLine(self):
        self.hide()
        leX1 = QtWidgets.QLineEdit()
        leX2 = QtWidgets.QLineEdit()
        leY1 = QtWidgets.QLineEdit()
        leY2 = QtWidgets.QLineEdit()
        X1, ok1 = QtWidgets.QInputDialog.getInt(self, 'Input x1',
                                                'Enter X1: ')
        Y1, ok2 = QtWidgets.QInputDialog.getInt(self, 'Input y1',
                                                'Enter Y1: ')
        X2, ok3 = QtWidgets.QInputDialog.getInt(self, 'Input x2',
                                                'Enter X2: ')
        Y2, ok4 = QtWidgets.QInputDialog.getInt(self, 'Input y2',
                                                'Enter Y2: ')

        if ok1 and ok2 and ok3 and ok4:
            DrawLineStep(int(X1) * 10 + x, y - int(Y1) * 10, int(X2) * 10 + x, y - int(Y2) * 10)
        self.show()

    def BresenhamLine(self):
        self.hide()
        leX1 = QtWidgets.QLineEdit()
        leX2 = QtWidgets.QLineEdit()
        leY1 = QtWidgets.QLineEdit()
        leY2 = QtWidgets.QLineEdit()
        X1, ok1 = QtWidgets.QInputDialog.getInt(self, 'Input x1',
                                                'Enter X1: ')
        Y1, ok2 = QtWidgets.QInputDialog.getInt(self, 'Input y1',
                                                'Enter Y1: ')
        X2, ok3 = QtWidgets.QInputDialog.getInt(self, 'Input x2',
                                                'Enter X2: ')
        Y2, ok4 = QtWidgets.QInputDialog.getInt(self, 'Input y2',
                                                'Enter Y2: ')

        if ok1 and ok2 and ok3 and ok4:
            DrawLineBresenham(int(X1) * 10 + x, y - int(Y1) * 10, int(X2) * 10 + x, y - int(Y2) * 10)
        self.show()

    def CDALine(self):
        self.hide()
        leX1 = QtWidgets.QLineEdit()
        leX2 = QtWidgets.QLineEdit()
        leY1 = QtWidgets.QLineEdit()
        leY2 = QtWidgets.QLineEdit()
        X1, ok1 = QtWidgets.QInputDialog.getInt(self, 'Input x1',
                                                'Enter X1: ')
        Y1, ok2 = QtWidgets.QInputDialog.getInt(self, 'Input y1',
                                                'Enter Y1: ')
        X2, ok3 = QtWidgets.QInputDialog.getInt(self, 'Input x2',
                                                'Enter X2: ')
        Y2, ok4 = QtWidgets.QInputDialog.getInt(self, 'Input y2',
                                                'Enter Y2: ')

        if ok1 and ok2 and ok3 and ok4:
            start = time.time()
            DrawCDA(int(X1) * 10 + x, y - int(Y1) * 10, int(X2) * 10 + x, y - int(Y2) * 10)
            finish = time.time()
            print("CDA Line", finish - start)
        self.show()

    def SmoothingLine(self):
        self.hide()
        leX1 = QtWidgets.QLineEdit()
        leX2 = QtWidgets.QLineEdit()
        leY1 = QtWidgets.QLineEdit()
        leY2 = QtWidgets.QLineEdit()
        X1, ok1 = QtWidgets.QInputDialog.getInt(self, 'Input x1',
                                                'Enter X1: ')
        Y1, ok2 = QtWidgets.QInputDialog.getInt(self, 'Input y1',
                                                'Enter Y1: ')
        X2, ok3 = QtWidgets.QInputDialog.getInt(self, 'Input x2',
                                                'Enter X2: ')
        Y2, ok4 = QtWidgets.QInputDialog.getInt(self, 'Input y2',
                                                'Enter Y2: ')

        if ok1 and ok2 and ok3 and ok4:
            smoothing(int(X1) * 10 + x, y - int(Y1) * 10, int(X2) * 10 + x, y - int(Y2) * 10)
        self.show()


def draw_cords(window):
    for i in range(0, WEIGHT // 10):
        pg.draw.line(window, OXY, (10 * i + 5, 0), (10 * i + 5, HEIGHT))
    for i in range(0, HEIGHT // 10):
        pg.draw.line(window, OXY, (0, 10 * i + 5), (WEIGHT, 10 * i + 5))
    pg.draw.line(window, BLACK, (0, y), (WEIGHT - 10, y), 2)
    pg.draw.line(window, BLACK, (x, 10), (x, HEIGHT), 2)
    pg.draw.polygon(window, BLACK, ((x - 7, 14), (x + 7, 14), (x, 7)))
    pg.draw.polygon(window, BLACK, ((WEIGHT - 14, y - 7), (WEIGHT - 14, y + 7), (WEIGHT - 7, y)))
    for i in range(0, WEIGHT, 10):
        pg.draw.line(window, BLACK, (i, y - 3), (i, y + 3))
    for i in range(0, HEIGHT, 10):
        pg.draw.line(window, BLACK, (x - 3, i), (x + 3, i))

    font = pg.font.Font('freesansbold.ttf', 13)
    Xtxt = font.render('X', True, BLACK, DARKGREY)
    Ytxt = font.render('Y', True, BLACK, DARKGREY)
    textRectX = Xtxt.get_rect()
    textRectY = Ytxt.get_rect()
    textRectX.center = (WEIGHT - 10, y - 15)
    textRectY.center = (x + 15, 15)
    window.blit(Xtxt, textRectX)
    window.blit(Ytxt, textRectY)


def mul(A, B):
    ans = [0, 0]
    for i in range(0, 2):
        for j in range(0, 2):
            ans[i] += A[i][j] * B[j]
    return ans


def draw(pos, w):
    pg.draw.rect(w, WHITE, pg.Rect(pos[0] - 5, pos[1] - 5, 10, 10))


def circle(R):
    cords = []
    xStep = 0
    yStep = R
    cords.append([xStep, -yStep])
    while abs(xStep) < abs(yStep):
        mW = abs((xStep + step) ** 2 + yStep ** 2 - R ** 2)
        mH = abs(xStep ** 2 + (yStep - step) ** 2 - R ** 2)
        mD = abs((xStep + step) ** 2 + (yStep - step) ** 2 - R ** 2)
        if min(mH, mW, mD) == mH:
            yStep -= step
        elif min(mH, mW, mD) == mW:
            xStep += step
        elif min(mH, mW, mD) == mD:
            xStep += step
            yStep -= step
        cords.append([xStep, yStep])
    matr1 = ((0, 1),
             (1, 0))
    matr2 = ((-1, 0),
             (0, 1))
    matr3 = ((1, 0),
             (0, -1))

    cords2 = []
    for i in cords:
        cords2.append(mul(matr1, i))
    for i in cords2:
        cords.append(i)
    cords2.clear()
    for i in cords:
        cords2.append(mul(matr2, i))
    for i in cords2:
        cords.append(i)
    cords2.clear()
    for i in cords:
        cords2.append(mul(matr3, i))
    for i in cords2:
        cords.append(i)
    cords2.clear()
    return cords


def StepLineGetCords(x1, y1, x2, y2):
    if x1 == x2:
        cords = [[x1, y1]]
        for i in range(min(y1, y2), max(y1, y2) + 10, 10):
            cords.append([x1, i])
        return cords
    k = (y2 - y1) / (x2 - x1)
    b = (y1 * (x2 - x1) - x1 * (y2 - y1)) / (x2 - x1)

    x1 = (x1 // 10) * 10
    x2 = (x2 // 10) * 10
    y1 = (y1 // 10) * 10
    y2 = (y2 // 10) * 10
    ans = [[x1, y1]]

    if abs(x2 - x1) > abs(y2 - y1):
        for i in range(x1, x2 + 10, 10):
            newY = i * k + b
            newY = (newY // 10) * 10
            ans.append([i, newY])
    else:
        for i in range(y2, y1 + 10, 10):
            newX = (i - b) / k
            newX = (newX // 10) * 10
            ans.append([newX, i])
    return ans


def BresenhamLineGetCords(x1, y1, x2, y2):
    if x1 != x2:
        alph = -(1 / 2)
        k = (y2 - y1) / (x2 - x1)
    else:
        cords = [[x1, y1]]
        for i in range(min(y1, y2), max(y1, y2) + 10, 10):
            cords.append([x1, i])
            return cords

    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1
    x1 = (x1 // 10) * 10
    x2 = (x2 // 10) * 10
    y1 = (y1 // 10) * 10
    ans = [[x1, y1]]

    if abs(x2 - x1) <=\
            abs(y2 - y1):
        c = 10
    else:
        c = 0

    for i in range(x1 + 10, x2 + 10, 10):
        if alph > 0:
            y1 += 10
            alph -= 1
        if alph < -1:
            y1 -= 10
            alph += 1
        ans.append([i, y1 - c])
        alph += k
    return ans


def DrawLineBresenham(x1, y1, x2, y2):
    pg.init()
    pg.display.set_caption("Line Bresenham")
    window = pg.display.set_mode((WEIGHT, HEIGHT))
    window.fill(DARKGREY)
    draw_cords(window)
    x1 = int(x1)
    x2 = int(x2)
    y1 = int(y1)
    y2 = int(y2)
    start = time.time()
    if abs(x2 - x1) > abs(y2 - y1):
        cords1 = BresenhamLineGetCords(x1, y1, x2, y2)
    else:
        matr = [[0, 1],
                [1, 0]]
        cords1 = BresenhamLineGetCords(y1, x1, y2, x2)
        for i in range(0, len(cords1)):
            cords1[i] = mul(matr, cords1[i])

    for i in cords1:
        draw((i[0], i[1]), window)
    pg.draw.line(window, BLUE, (x1, y1), (x2, y2), 3)
    finish = time.time()
    print("LineBresenham ", finish - start)
    pg.display.flip()
    running = True
    while running:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
                pg.quit()


def DrawLineStep(x1, y1, x2, y2):
    pg.init()
    pg.display.set_caption("Line Step")
    window = pg.display.set_mode((WEIGHT, HEIGHT))
    window.fill(DARKGREY)
    draw_cords(window)
    x1 = int(x1)
    x2 = int(x2)
    y1 = int(y1)
    y2 = int(y2)
    start = time.time()
    if x2 < x1:
        x2, x1 = x1, x2
        y2, y1 = y1, y2
    cords1 = StepLineGetCords(x1, y1, x2, y2)
    for j in cords1:
        draw((j[0], j[1]), window)
    pg.draw.line(window, BLUE, (x1, y1), (x2, y2), 3)
    finish = time.time()
    print("Line step ", finish - start)
    pg.display.flip()
    running = True
    while running:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
                pg.quit()


def DrawCDA(x1, y1, x2, y2):
    pg.init()
    pg.display.set_caption("Line CDA")
    window = pg.display.set_mode((WEIGHT, HEIGHT))
    window.fill(DARKGREY)
    draw_cords(window)
    x1 = int(x1)
    x2 = int(x2)
    y1 = int(y1)
    y2 = int(y2)
    start = time.time()
    cords1 = CDA(x1, y1, x2, y2)
    for j in cords1:
        draw((j[0], j[1]), window)
    pg.draw.line(window, BLUE, (x1, y1), (x2, y2), 3)
    finish = time.time()
    print("CDA Line", finish - start)
    pg.display.flip()
    running = True
    while running:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
                pg.quit()


def DrawCircle(R):
    pg.init()
    pg.display.set_caption("Circle")
    window = pg.display.set_mode((WEIGHT, HEIGHT))
    window.fill(DARKGREY)
    draw_cords(window)
    start = time.time()
    cords1 = circle(R)
    for j in cords1:
        draw((x + j[0], y - j[1]), window)
    pg.draw.circle(window, BLUE, MID, R, 3)
    finish = time.time()
    print("Circle", finish - start)
    pg.display.flip()
    running = True
    while running:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
                pg.quit()


def CDA(x1, y1, x2, y2):
    Dx = (x2 - x1)
    Dy = (y2 - y1)
    if abs(Dy) > abs(Dx):
        L = Dy
        step = -10
    else:
        L = Dx
        step = 10
    ans = [[x1 // 10 * 10, y1 // 10 * 10]]
    for i in range(0, L, step):
        x1 = x1 + (Dx / L) * step
        y1 = y1 + (Dy / L) * step
        ans.append([x1 // 10 * 10, y1 // 10 * 10])
    return ans


def smoothing(x1, y1, x2, y2):
    x1 = int(x1)
    x2 = int(x2)
    y1 = int(y1)
    y2 = int(y2)

    if abs(x2 - x1) > abs(y2 - y1):
        cords1 = BresenhamLineGetCords(x1, y1, x2, y2)
    else:
        matr = [[0, 1],
                [1, 0]]
        cords1 = BresenhamLineGetCords(y1, x1, y2, x2)
        for i in range(0, len(cords1)):
            cords1[i] = mul(matr, cords1[i])

    pg.init()
    pg.display.set_caption("smoothing")
    window = pg.display.set_mode((WEIGHT, HEIGHT))
    window.fill(DARKGREY)
    draw_cords(window)

    start = time.time()


    if x1 != x2:
        k = (y2 - y1) / (x2 - x1)
        b = (y1 * (x2 - x1) - x1 * (y2 - y1)) / (x2 - x1)

    for i in range(0, len(cords1)):
        if x1 != x2:
            if abs(x2 - x1) > abs(y2 - y1):
                dif = cords1[i][1] - (cords1[i][0] * k + b)
            else:
                dif = cords1[i][0] - ((cords1[i][1] - b) / k)
        else:
            dif = 0
        if dif != 0 and x1 != x2:
            color = (
                70 + min(abs(150 * (1 / dif)), 150), 70 + min(abs(150 * (1 / dif)), 150),
                70 + min(abs(150 * (1 / dif)), 150))
        else:
            color = (220, 220, 220)
        pg.draw.rect(window, color,
                     pg.Rect(cords1[i][0] - 5, cords1[i][1] - 5, 10, 10))
    pg.draw.line(window, BLUE, (x1, y1), (x2, y2), 3)
    finish = time.time()
    print("smoothing ", finish - start)
    pg.display.flip()
    running = True
    while running:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
                pg.quit()
