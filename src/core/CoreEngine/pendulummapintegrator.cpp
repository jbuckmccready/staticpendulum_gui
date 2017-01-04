/* ===========================================================================
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jedidiah Buck McCready <jbuckmccready@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ===========================================================================*/
#include "pendulummapintegrator.h"
#include <cmath>

namespace staticpendulum {
Map::Map(double xStart, double yStart, double xEnd, double yEnd,
             double resolution)
    : m_xStart{xStart}, m_yStart{yStart}, m_xEnd{xEnd}, m_yEnd{yEnd},
      m_resolution{resolution} {
  // column and row count, +1 to make it an inclusive range
  m_cols = std::abs(std::lround((m_xEnd - m_xStart) / m_resolution)) + 1;
  m_rows = std::abs(std::lround((m_yEnd - m_yStart) / m_resolution)) + 1;

  m_mapData.resize(m_rows * m_cols);

  // create int multipliers to fill data to avoid floating math rounding error
  int xdim_factor = std::lround(m_xStart / m_resolution);
  int ydim_factor = std::lround(m_yStart / m_resolution);

  for (std::size_t i = 0; i < m_rows; ++i) {
    for (std::size_t j = 0; j < m_cols; ++j) {
      // data is row major oriented
      std::size_t index = i * m_cols + j;

      m_mapData[index].xPosition =
          static_cast<double>(xdim_factor) * m_resolution;

      // y factor is negative such that the first element corresponds to the
      // upper left of the map
      m_mapData[index].yPosition =
          static_cast<double>(ydim_factor) * -m_resolution;

      m_mapData[index].xVelocity = 0.0;
      m_mapData[index].yVelocity = 0.0;
      ++xdim_factor;
    }

    // reset x value for next row
    xdim_factor = std::lround(m_xStart / m_resolution);
    ++ydim_factor;
  }
}
} // namespace staticpendulum
