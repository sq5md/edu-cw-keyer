#!/usr/bin/env python3
from PIL import Image, ImageDraw

def dot_duration_seconds(wpm: int) -> float:
    """
    Zwraca czas trwania kropki Morse'a w sekundach
    dla danego WPM (PARIS standard).
    """
    return 60.0 / (50.0 * wpm)

def generate_latex_morse_table(wpm_min=5, wpm_max=35):
    lines = []

    lines.append(r"\begin{tabular}{|c|c|}")
    lines.append(r"\hline")
    lines.append(r"WPM & Czas kropki (s) \\")
    lines.append(r"\hline")

    for wpm in range(wpm_min, wpm_max + 1):
        dot = dot_duration_seconds(wpm)
        lines.append(f"{wpm} & {dot:.4f} \\\\")
        lines.append(r"\hline")


    lines.append(r"\hline")
    lines.append(r"\end{tabular}")

    return "\n".join(lines)


TOTAL_UNITS = 50
CELL_SIZE = 20
MARGIN = 10

MORSE = {
    'A': '.-',    'B': '-...',  'C': '-.-.',
    'D': '-..',   'E': '.',     'F': '..-.',
    'G': '--.',   'H': '....',  'I': '..',
    'J': '.---',  'K': '-.-',   'L': '.-..',
    'M': '--',    'N': '-.',    'O': '---',
    'P': '.--.',  'Q': '--.-',  'R': '.-.',
    'S': '...',   'T': '-',     'U': '..-',
    'V': '...-',  'W': '.--',   'X': '-..-',
    'Y': '-.--',  'Z': '--..'
}

text = "PARIS "

timeline = []
separators = []

for idx, ch in enumerate(text):

    # spacja = przerwa między wyrazami
    if ch == ' ':
        timeline.extend([0] * 7)

        # separator po zakończeniu spacji
        separators.append(len(timeline))
        continue

    code = MORSE[ch]

    for symbol_idx, symbol in enumerate(code):

        if symbol == '.':
            timeline.append(1)
        else:
            timeline.extend([1, 1, 1])

        # odstęp między elementami litery
        if symbol_idx < len(code) - 1:
            timeline.append(0)

    # separator po literze
    separators.append(len(timeline))

    # jeśli następny znak istnieje i nie jest spacją,
    # dodaj przerwę między literami
    if idx < len(text) - 1 and text[idx + 1] != ' ':
        timeline.extend([0, 0, 0])

        # separator po przerwie między literami
        separators.append(len(timeline))


timeline = timeline[:TOTAL_UNITS]
timeline.extend([0] * (TOTAL_UNITS - len(timeline)))

width = TOTAL_UNITS * CELL_SIZE + 2 * MARGIN
height = CELL_SIZE + 2 * MARGIN

img = Image.new("RGB", (width, height), "white")
draw = ImageDraw.Draw(img)

# pola czasu
for i, value in enumerate(timeline):
    x0 = MARGIN + i * CELL_SIZE
    y0 = MARGIN
    x1 = x0 + CELL_SIZE - 1
    y1 = y0 + CELL_SIZE - 1

    draw.rectangle(
        [x0, y0, x1, y1],
        fill="black" if value else "white",
        outline="gray"
    )

# czerwone separatory
for pos in separators:
    if pos <= TOTAL_UNITS:
        x = MARGIN + pos * CELL_SIZE

        draw.line(
            [(x, MARGIN - 5),
             (x, MARGIN + CELL_SIZE + 5)],
            fill="red",
            width=3
        )

# opcjonalnie: czcionka (domyślna PIL)
try:
    from PIL import ImageFont
    font = ImageFont.load_default()
except:
    font = None

# numery nad polami
for i in range(TOTAL_UNITS):
    x = MARGIN + i * CELL_SIZE
    y = MARGIN - 12  # lekko nad kwadratami

    draw.text(
        (x + 3, y),
        str(i+1),
        fill="black",
        font=font
    )

img.save("morse_timeline.png")
print("Zapisano morse_timeline.png")


table = generate_latex_morse_table(5, 35)

print(table)
