#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* WEBPAGE_HTML = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Race Car Control</title>
  <style>
    * { margin:0; padding:0; box-sizing:border-box; }
    body {
      background: #0f172a;
      color: #f1f5f9;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      height: 100vh;
      width: 100vw;
      overflow: hidden;
      display: flex;
      flex-direction: row;
    }

    .panel {
      flex: 1;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 8px;
    }
    .panel-left {
      background: linear-gradient(160deg, #0f172a, #1a1040);
      border-right: 1px solid rgba(99,102,241,0.15);
    }
    .panel-right {
      background: linear-gradient(200deg, #0f172a, #0a1f2e);
    }

    .label {
      font-size: 11px;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 3px;
      color: #475569;
      margin-bottom: 6px;
    }
    .val {
      font-size: 22px;
      font-weight: 800;
      margin-bottom: 10px;
      text-align: center;
    }
    .val-motor { color: #818cf8; }
    .val-steer { color: #34d399; }

    /* ── Vertical slider (throttle) ── */
    .v-wrap {
      height: 120vh;
      width: 120px; /* Wider grab area */
      display: flex;
      justify-content: center;
      align-items: center;
    }
    .v-slider {
      width: 120vh;
      -webkit-transform: rotate(-90deg);
      -moz-transform: rotate(-90deg);
      transform: rotate(-90deg);
    }

    /* ── Horizontal slider (steering) ── */
    .h-wrap {
      width: 80%;
      max-width: 400px;
      height: 70px;
      display: flex;
      justify-content: center;
      align-items: center;
    }
    .h-slider { width: 100%; }

    /* ── Slider base ── */
    input[type=range] {
      -webkit-appearance: none;
      -moz-appearance: none;
      appearance: none;
      background: transparent;
      width: 100%;
      outline: none;
    }

    /* Track - Webkit */
    input[type=range]::-webkit-slider-runnable-track {
      height: 12px;
      background: linear-gradient(90deg, rgba(255,255,255,0.04), rgba(255,255,255,0.08), rgba(255,255,255,0.04));
      border-radius: 6px;
      border: 1px solid rgba(255,255,255,0.06);
    }
    /* Track - Firefox */
    input[type=range]::-moz-range-track {
      height: 12px;
      background: linear-gradient(90deg, rgba(255,255,255,0.04), rgba(255,255,255,0.08), rgba(255,255,255,0.04));
      border-radius: 6px;
      border: 1px solid rgba(255,255,255,0.06);
    }

    /* Thumb - Webkit */
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      height: 44px;
      width: 44px;
      border-radius: 50%;
      border: 2px solid rgba(255,255,255,0.2);
      margin-top: -17px;
      cursor: pointer;
    }
    /* Thumb - Firefox */
    input[type=range]::-moz-range-thumb {
      height: 40px;
      width: 40px;
      border-radius: 50%;
      border: 2px solid rgba(255,255,255,0.2);
      cursor: pointer;
    }

    /* Motor thumb */
    .v-slider::-webkit-slider-thumb {
      height: 80px;
      width: 80px;
      margin-top: -34px;
      background: radial-gradient(circle at 38% 38%, #a5b4fc, #6366f1);
      box-shadow: 0 0 16px rgba(99,102,241,0.6);
    }
    .v-slider::-moz-range-thumb {
      height: 80px;
      width: 80px;
      background: radial-gradient(circle at 38% 38%, #a5b4fc, #6366f1);
      box-shadow: 0 0 16px rgba(99,102,241,0.6);
    }
    .v-slider::-webkit-slider-runnable-track { height: 24px; border-radius: 12px; }
    .v-slider::-moz-range-track { height: 24px; border-radius: 12px; }

    /* Steer thumb */
    .h-slider::-webkit-slider-thumb {
      background: radial-gradient(circle at 38% 38%, #6ee7b7, #10b981);
      box-shadow: 0 0 12px rgba(16,185,129,0.5);
    }
    .h-slider::-moz-range-thumb {
      background: radial-gradient(circle at 38% 38%, #6ee7b7, #10b981);
      box-shadow: 0 0 12px rgba(16,185,129,0.5);
    }
  </style>
</head>
<body>

  <div class="panel panel-left">
    <div class="label">Throttle</div>
    <div class="val val-motor" id="mVal">0%</div>
    <div class="v-wrap">
      <input type="range" min="-255" max="255" value="0" class="v-slider" id="mSlider">
    </div>
  </div>

  <div class="panel panel-right">
    <div class="label">Steering</div>
    <div class="val val-steer" id="sVal">0&deg;</div>
    <div class="h-wrap">
      <input type="range" min="22" max="120" value="72" dir="rtl" class="h-slider" id="sSlider">
    </div>
  </div>

  <script>
    var mSlider = document.getElementById('mSlider');
    var sSlider = document.getElementById('sSlider');
    var mVal = document.getElementById('mVal');
    var sValEl = document.getElementById('sVal');
    var SC = 72, MS = 0;
    var mTO = null, sTO = null;

    function send(a, v, fin) {
      if (a === 's') {
        if (!sTO || fin) {
          var x = new XMLHttpRequest();
          x.open('GET', '/control?steer=' + v, true);
          x.send();
          if (!fin) sTO = setTimeout(function(){ sTO = null; }, 50);
        }
      } else {
        if (!mTO || fin) {
          var x = new XMLHttpRequest();
          x.open('GET', '/control?motor=' + v, true);
          x.send();
          if (!fin) mTO = setTimeout(function(){ mTO = null; }, 50);
        }
      }
    }

    mSlider.addEventListener('input', function() {
      var v = parseInt(mSlider.value);
      var p = Math.round(v / 255 * 100);
      mVal.textContent = (p > 0 ? '+' : '') + p + '%';
      send('m', v);
    });

    sSlider.addEventListener('input', function() {
      var v = parseInt(sSlider.value);
      var d = Math.round((v - SC) * (45 / (120 - SC)));
      sValEl.innerHTML = (d > 0 ? '+' : '') + d + '&deg;';
      send('s', v);
    });

    function rstM() {
      mSlider.value = MS;
      mVal.textContent = '0%';
      send('m', MS, true);
    }
    function rstS() {
      sSlider.value = SC;
      sValEl.innerHTML = '0&deg;';
      send('s', SC, true);
    }

    mSlider.addEventListener('change', rstM);
    mSlider.addEventListener('mouseup', rstM);
    mSlider.addEventListener('touchend', rstM);
    sSlider.addEventListener('change', rstS);
    sSlider.addEventListener('mouseup', rstS);
    sSlider.addEventListener('touchend', rstS);
  </script>
</body>
</html>
)=====";

#endif
