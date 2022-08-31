import random
import re

from faker import Faker

root = "data/"
lessons = list(
    map(
        lambda s: re.sub(" ", "-", s).lower(),
        [
            "College English",
            "Advanced Mathematics",
            "Military Theory",
            "Mechanical Graphing",
            "Algorithmic Language",
            "College Physics",
            "Experiment of College Physics",
            "Linear Algebra",
            "Fundamentals of Law",
            "General Physics",
            "Lab of General Physics",
            "Functions of Complex Variables Integral Transformations",
            "Theory of Circuitry",
            "Circuit Measurement Technology",
            "Probability Theory Stochastic Process",
            "Signal Linear System",
            "Circuitry",
            "Pulse & Numerical Circuitry",
            "Metalworking Practice",
            "Electrical Engineering Practice",
            "College English Test (Band4)",
            "Experiment in Electronic Circuitry",
            "Principle of Microcomputer",
            "Electromagnetic Fields & Magnetic Waves",
            "Computational Method",
            "Basis of Software Technique",
            "Microwave Technique",
            "Principle of Communication",
            "Digital Signal Processing",
            "Experiment of Microcomputer",
            "Computer Interface Technology",
            "C language",
            "College English Test (Band6)",
            "Industrial Enterprise Management",
            "Moving Communication",
            "Fiber Optical Communication System",
            "Introduction to Reliability Technology",
            "Satellite Communications",
            "Television Operation",
            "Digital Image Processing",
            "Specialty English",
            "Information Searches",
            "Graduation Thesis",
            "Automatic Control Theory",
            "Analogical Electronics",
            "Digital Electronics",
            "Economy of Capitalism",
            "Principle of Marxism",
            "Principle of Mechanic",
            "Mechanic Design",
            "Optimum Control",
            "Microcomputer Control Technology",
            "Procedure Control",
            "Automatic Control System",
            "Semiconductor converting Technique",
            "Operational Research",
            "Auto-Measurement Technique",
            "Principle of Sensing Device",
            "Principle of Single-Chip computer",
            "python",
            "math",
        ],
    )
)
with open(f"{root}lessons.txt", "w") as f:
    T = len(lessons)
    f.write(f"{T}\n")
    for i, ls in enumerate(lessons, start=1):
        f.write(
            f'{ls} {random.randrange(1, 6)} {"%06d" % i} https://www.usst.com/{ls} {"english" if "english" in ls else "math" if "math" in ls else "computer-science"}\n'
        )
faker = Faker("")


def chg(s):
    return re.sub(" ", "-", s)


activities = ["class-meeting", "activity"]
with open(f"{root}students.txt", "w") as f:
    for _ in range(100):
        f.write(
            f"{random.randint(1, 1000000)} {chg(faker.name())} cs {'male' if random.randint(0, 1) == 0 else 'female'} {random.randint(1998, 2003)} 2020\n"
        )
        T = random.randint(1, 30)
        for _ in range(T):
            ls = lessons[random.randint(0, len(lessons) - 1)]
            f.write(
                f"{ls} {'%.1f' % (random.random() * 40 + 60)} {chg(faker.name())} room-{'%d%02d' % (random.randint(1, 9), random.randint(0, 12))}\n"
            )
        f.write("null 0 null null\n")
        T = random.randint(1, 6)
        for _ in range(T):
            ls = f"{activities[random.randint(0, 1)]}-{random.randint(1, 3)}"
            f.write(f"{ls} {random.randint(1, 3)}\n")
        f.write("null 0\n")
