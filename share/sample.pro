;; Copyright (C) 2015 Chi-kwan Chan
;; Copyright (C) 2015 Steward Observatory
;;
;; This file is part of insight.
;;
;; Insight is free software: you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; Insight is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with insight.  If not, see <http://www.gnu.org/licenses/>.

pro sample

  d = bytarr(4,128,128,128)
  z = (dindgen(128) - 63.5) / 63.5

  for j = 0, 127 do begin
    y = z[j]
    for i = 0, 127 do begin
      x = z[i]
      f = exp(-0.5 * (x*x + y*y + z*z) / 0.25)
      d[0,*,j,i] = 255 * (x + 1) / 2
      d[1,*,j,i] = 255 * (y + 1) / 2
      d[2,*,j,i] = 255 * (z + 1) / 2
      d[3,*,j,i] =  16 * f
    endfor
  endfor

  openw,  lun, 'sample.raw', /get_lun
  writeu, lun, d
  close,  lun & free_lun

end
