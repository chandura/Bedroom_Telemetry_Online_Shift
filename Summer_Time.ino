void clockchange () {
  if (themonth == 10) {
    if (theday == 0) {
      if (monthday + 7 > 31) {
        GMT = "Y";
      }
    }
  }
  if (themonth == 3) {
    if (theday == 0) {
      if (monthday + 7 > 31) {
        GMT = "N";
      }
    }
  }
}

