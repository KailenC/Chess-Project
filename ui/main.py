from PyQt6.QtWidgets import QApplication, QWidget  # type: ignore
from PyQt6.QtGui import QPainter, QColor  # type: ignore
import sys


class Board(QWidget):
    def paintEvent(self, event):
        painter = QPainter(self)

        size = 80

        for r in range(8):
            for c in range(8):
                color = (
                    QColor(240, 217, 181) if (r + c) % 2 == 0 else QColor(181, 136, 99)
                )
                painter.fillRect(c * size, r * size, size, size, color)


app = QApplication(sys.argv)
window = Board()
window.resize(640, 640)
window.show()
sys.exit(app.exec())
