""" python deps for this project """

import config.shared

build_requires: list[str] = config.shared.BUILD
test_requires: list[str] = config.shared.TEST
requires = build_requires + test_requires
