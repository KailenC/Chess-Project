from PyQt6.QtWidgets import QApplication, QWidget
from PyQt6.QtGui import QPainter, QColor, QPixmap
import subprocess
import sys

engine = subprocess.Popen(
    ["engine/build/engine.exe"],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True,
)

engine.stdin.write("hello\n")
engine.stdin.flush()

response = engine.stdout.readline()

print(response)


class Board(QWidget):
    def __init__(self):
        super().__init__()

        self.white_pawn = QPixmap("ui/Pieces/pawn-w.png")

    def paintEvent(self, event):
        painter = QPainter(self)

        size = 80

        for r in range(8):
            for c in range(8):
                color = (
                    QColor(240, 217, 181) if (r + c) % 2 == 0 else QColor(181, 136, 99)
                )
                painter.fillRect(c * size, r * size, size, size, color)

        painter.drawPixmap(0, 6 * size, size, size, self.white_pawn)


app = QApplication(sys.argv)
window = Board()
window.resize(640, 640)
window.show()
sys.exit(app.exec())
