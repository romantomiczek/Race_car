#ifndef WEBPAGE_H
#define WEBPAGE_H

const char *WEBPAGE_HTML = R"=====(
<!doctype html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
  <title>Race Car Control</title>
  <style>
    /* === Reset & Base === */
    *{box-sizing:border-box;margin:0;padding:0}
    html,body{height:100%;overflow:hidden}
    body{
      font-family:system-ui,-apple-system,"Segoe UI",Roboto,Arial,sans-serif;
      background:radial-gradient(ellipse at 20% 0%,#1a2d4a,#0d1520 50%,#080c12);
      color:#e6edf5;
      display:flex;
      flex-direction:column;
      height:100dvh;
      height:100vh;
      user-select:none;
      -webkit-user-select:none;
      -webkit-touch-callout:none;
      -webkit-tap-highlight-color:transparent;
    }

    /* === Header === */
    header{
      display:flex;
      align-items:center;
      justify-content:space-between;
      padding:8px 12px;
      flex-shrink:0;
      border-bottom:1px solid rgba(147,173,204,.15);
      background:rgba(10,18,28,.6);
      backdrop-filter:blur(12px);
      -webkit-backdrop-filter:blur(12px);
      z-index:10;
      gap:8px;
    }
    .title-area{display:flex;align-items:center;gap:8px;flex-shrink:0}
    .title-area h1{font-size:1.05rem;font-weight:700;letter-spacing:.03em;white-space:nowrap}
    .status-dot{
      width:12px;height:12px;border-radius:50%;flex-shrink:0;
      transition:background .3s,box-shadow .3s;
    }
    .status-dot.online{
      background:#57e389;
      box-shadow:0 0 0 4px rgba(87,227,137,.15),0 0 12px rgba(87,227,137,.3);
    }
    .status-dot.offline{
      background:#ff5c70;
      box-shadow:0 0 0 4px rgba(255,92,112,.15),0 0 12px rgba(255,92,112,.3);
    }

    /* === Mode Toggle === */
    .mode-toggle{
      display:flex;
      background:rgba(20,32,48,.7);
      border:1px solid rgba(147,173,204,.2);
      border-radius:10px;
      padding:3px;
      gap:2px;
      flex-shrink:1;
      min-width:0;
    }
    .mode-btn{
      padding:6px 12px;
      border:none;border-radius:8px;
      background:transparent;
      color:#6b82a0;
      font-size:.78rem;font-weight:600;
      cursor:pointer;
      transition:all .2s ease;
      white-space:nowrap;
    }
    .mode-btn.active{
      background:linear-gradient(135deg,rgba(63,127,210,.4),rgba(41,88,150,.4));
      color:#e6edf5;
      box-shadow:0 2px 8px rgba(0,0,0,.2);
    }

    /* === Settings Button === */
    .settings-btn{
      width:38px;height:38px;
      border:1px solid rgba(147,173,204,.25);
      border-radius:10px;
      background:rgba(30,45,65,.5);
      color:#b6c6d9;font-size:1.15rem;
      cursor:pointer;
      display:flex;align-items:center;justify-content:center;
      transition:background .2s,border-color .2s;
      flex-shrink:0;
    }
    .settings-btn:active{
      background:rgba(50,70,95,.6);
      border-color:rgba(134,185,255,.4);
    }

    /* === Control Area === */
    .control-area{
      flex:1 1 0;display:flex;position:relative;overflow:hidden;min-height:0;
    }
    .view{width:100%;height:100%;display:flex}

    /* === Joystick View === */
    #joystickView{
      align-items:center;justify-content:center;
      flex-direction:column;gap:12px;padding:12px;
    }
    .joystick{
      width:min(68vmin,300px);height:min(68vmin,300px);
      border-radius:50%;
      border:2px solid rgba(147,173,204,.3);
      background:radial-gradient(circle,#1e3250 0%,#152438 55%,#0f1a28 100%);
      position:relative;touch-action:none;
      box-shadow:inset 0 0 30px rgba(0,0,0,.3),0 8px 32px rgba(0,0,0,.25);
    }
    .joy-line{position:absolute;background:rgba(147,173,204,.1)}
    .joy-line-x{left:15%;right:15%;top:50%;height:1px}
    .joy-line-y{top:15%;bottom:15%;left:50%;width:1px}
    .knob{
      width:34%;height:34%;border-radius:50%;
      background:radial-gradient(circle at 38% 32%,#a0cfff,#5a9de0 30%,#3a78c0 60%,#295896 100%);
      position:absolute;left:33%;top:33%;
      box-shadow:0 4px 16px rgba(0,0,0,.35),0 0 20px rgba(134,185,255,.15);
      pointer-events:none;
    }
    .readout{display:flex;gap:8px}

    /* === Sliders View === */
    #slidersView{flex-direction:row}
    .slider-zone{
      flex:1;display:flex;flex-direction:column;
      align-items:center;justify-content:center;
      padding:10px 8px;touch-action:none;position:relative;
      transition:background .15s;
    }
    .slider-zone.active{background:rgba(134,185,255,.03)}
    .zone-divider{width:1px;background:rgba(147,173,204,.12);flex-shrink:0}
    .zone-label{
      font-size:.7rem;font-weight:700;letter-spacing:.12em;
      color:#5a7490;text-transform:uppercase;
      flex-shrink:0;margin-bottom:6px;
    }
    .zone-value{
      font-size:1.05rem;font-weight:700;color:#d1deed;
      flex-shrink:0;margin-top:6px;min-height:1.4em;
    }

    /* === Vertical Track (Throttle) === */
    .track-v{
      width:14px;flex:1;max-height:420px;
      border-radius:7px;
      background:rgba(12,20,32,.9);
      border:1px solid rgba(147,173,204,.2);
      position:relative;
      box-shadow:inset 0 2px 6px rgba(0,0,0,.3);
    }
    .track-v::after{
      content:'';position:absolute;
      left:-12px;right:-12px;top:50%;height:2px;
      background:rgba(147,173,204,.13);
      transform:translateY(-1px);border-radius:1px;
    }
    .track-fill-v{
      position:absolute;left:1px;right:1px;border-radius:6px;
      transition:background .12s;
    }
    .track-fill-v.fwd{
      background:linear-gradient(to top,rgba(87,227,137,.25),rgba(87,227,137,.6));
    }
    .track-fill-v.rev{
      background:linear-gradient(to bottom,rgba(255,92,112,.25),rgba(255,92,112,.6));
    }

    /* === Horizontal Track (Steering) === */
    .track-h-wrap{
      flex:1;display:flex;align-items:center;width:100%;padding:0 4px;
    }
    .track-h{
      height:14px;width:100%;border-radius:7px;
      background:rgba(12,20,32,.9);
      border:1px solid rgba(147,173,204,.2);
      position:relative;
      box-shadow:inset 0 2px 6px rgba(0,0,0,.3);
    }
    .track-h::after{
      content:'';position:absolute;
      top:-12px;bottom:-12px;left:50%;width:2px;
      background:rgba(147,173,204,.13);
      transform:translateX(-1px);border-radius:1px;
    }
    .track-fill-h{
      position:absolute;top:1px;bottom:1px;border-radius:6px;
      background:linear-gradient(90deg,rgba(134,185,255,.25),rgba(134,185,255,.6));
    }

    /* === Track Knob (shared) === */
    .track-knob{
      width:50px;height:50px;border-radius:50%;
      background:radial-gradient(circle at 38% 32%,#a0cfff,#5a9de0 30%,#3a78c0 60%,#295896 100%);
      position:absolute;
      box-shadow:0 4px 14px rgba(0,0,0,.35),0 0 14px rgba(134,185,255,.12);
      pointer-events:none;z-index:2;
    }
    .track-v .track-knob{
      left:50%;top:50%;transform:translate(-50%,-50%);
    }
    .track-h .track-knob{
      top:50%;left:50%;transform:translate(-50%,-50%);
    }

    /* === Status Bar === */
    .status-bar{
      display:flex;justify-content:center;gap:8px;
      padding:6px 12px;flex-shrink:0;
      border-top:1px solid rgba(147,173,204,.1);
      background:rgba(10,18,28,.5);
    }
    .chip{
      background:rgba(50,70,100,.22);
      border:1px solid rgba(147,173,204,.15);
      border-radius:8px;padding:3px 10px;
      font-size:.78rem;color:#8fa3ba;
    }

    /* === Settings Modal === */
    .modal-overlay{
      position:fixed;inset:0;
      background:rgba(0,0,0,.55);
      backdrop-filter:blur(6px);
      -webkit-backdrop-filter:blur(6px);
      z-index:100;
      display:flex;align-items:flex-end;justify-content:center;
      opacity:0;pointer-events:none;
      transition:opacity .25s ease;
    }
    .modal-overlay.active{opacity:1;pointer-events:auto}
    .modal-sheet{
      background:linear-gradient(180deg,#1a2a42,#0f1a28);
      border:1px solid rgba(147,173,204,.22);
      border-bottom:none;
      border-radius:20px 20px 0 0;
      width:100%;max-width:480px;max-height:80vh;
      overflow-y:auto;-webkit-overflow-scrolling:touch;
      transform:translateY(100%);
      transition:transform .3s cubic-bezier(.22,1,.36,1);
    }
    .modal-overlay.active .modal-sheet{transform:translateY(0)}
    .modal-handle{
      width:36px;height:4px;border-radius:2px;
      background:rgba(147,173,204,.3);margin:10px auto 0;
    }
    .modal-header{
      display:flex;justify-content:space-between;align-items:center;
      padding:12px 20px 8px;
    }
    .modal-header h2{font-size:1.1rem;font-weight:700}
    .close-btn{
      width:34px;height:34px;
      border:1px solid rgba(147,173,204,.2);border-radius:50%;
      background:rgba(30,45,65,.5);color:#b6c6d9;
      font-size:1rem;cursor:pointer;
      display:flex;align-items:center;justify-content:center;
    }
    .close-btn:active{background:rgba(50,70,95,.6)}
    .modal-body{padding:0 20px;display:grid;gap:14px}
    .setting-head{
      display:flex;justify-content:space-between;align-items:center;
      margin-bottom:6px;color:#d1deed;
    }
    .setting-head label{font-size:.88rem}
    .setting-head strong{font-size:.95rem;color:#fff}
    .modal-body input[type=range]{width:100%;accent-color:#86b9ff;height:28px}
    .modal-footer{padding:14px 20px 28px;display:grid;gap:8px}
    .settings-status{
      font-size:.82rem;color:#b6c6d9;min-height:1.2em;text-align:center;
    }
    .save-btn{
      width:100%;border:1px solid rgba(182,219,255,.34);
      border-radius:14px;padding:13px;
      background:linear-gradient(135deg,#bfe0ff 0%,#7fb7ff 34%,#4f8be0 100%);
      color:#06101b;font-size:.92rem;font-weight:800;
      cursor:pointer;
      box-shadow:0 12px 24px rgba(25,67,128,.25),inset 0 1px 0 rgba(255,255,255,.4);
      transition:transform .15s,box-shadow .15s;
    }
    .save-btn:active{transform:scale(.97)}
    .save-btn:disabled{opacity:.6;cursor:wait;transform:none}

    /* === Toast === */
    .toast{
      position:fixed;left:50%;bottom:20px;
      transform:translate(-50%,16px);
      padding:10px 16px;border-radius:12px;
      background:rgba(8,17,29,.94);
      border:1px solid rgba(147,173,204,.2);
      color:#eef6ff;font-size:.85rem;
      box-shadow:0 10px 28px rgba(0,0,0,.3);
      opacity:0;pointer-events:none;
      transition:opacity .2s,transform .2s;
      z-index:200;max-width:min(92vw,380px);text-align:center;
    }
    .toast.show{opacity:1;transform:translate(-50%,0)}
    .toast.ok{background:linear-gradient(135deg,rgba(31,76,48,.96),rgba(20,44,30,.96));border-color:rgba(127,220,156,.35)}
    .toast.err{background:linear-gradient(135deg,rgba(107,35,35,.96),rgba(57,18,18,.96));border-color:rgba(255,140,140,.3)}

    /* === Landscape tweaks === */
    @media(orientation:landscape){
      header{padding:4px 12px}
      .title-area h1{font-size:.95rem}
      .mode-btn{padding:4px 10px;font-size:.74rem}
      .settings-btn{width:32px;height:32px;font-size:1rem;border-radius:8px}
      .status-bar{padding:3px 12px}
      .chip{padding:2px 8px;font-size:.72rem}
      .zone-label{font-size:.65rem;margin-bottom:4px}
      .zone-value{font-size:.9rem;margin-top:4px}
      .joystick{width:min(65vmin,260px);height:min(65vmin,260px)}
      #joystickView{gap:6px;padding:6px}
    }
  </style>
</head>
<body>

  <!-- Header -->
  <header>
    <div class="title-area">
      <span id="statusDot" class="status-dot offline"></span>
      <h1>Race Car</h1>
    </div>
    <div class="mode-toggle">
      <button id="joyModeBtn" class="mode-btn active" type="button">&#x1F579; Joystick</button>
      <button id="sliderModeBtn" class="mode-btn" type="button">&#x2195; Sliders</button>
    </div>
    <button id="settingsBtn" class="settings-btn" type="button" aria-label="Settings">&#9881;</button>
  </header>

  <!-- Control Area -->
  <main class="control-area">

    <!-- Joystick View -->
    <div id="joystickView" class="view">
      <div class="joystick" id="joy">
        <div class="joy-line joy-line-x"></div>
        <div class="joy-line joy-line-y"></div>
        <div class="knob" id="knob"></div>
      </div>
      <div class="readout">
        <div class="chip">X: <span id="joyXVal">0</span></div>
        <div class="chip">Y: <span id="joyYVal">0</span></div>
      </div>
    </div>

    <!-- Sliders View -->
    <div id="slidersView" class="view" style="display:none">
      <div class="slider-zone" id="throttleZone">
        <span class="zone-label">Throttle</span>
        <div class="track-v" id="throttleTrack">
          <div class="track-fill-v" id="throttleFill"></div>
          <div class="track-knob" id="throttleKnob"></div>
        </div>
        <span class="zone-value" id="throttleVal">0%</span>
      </div>
      <div class="zone-divider"></div>
      <div class="slider-zone" id="steeringZone">
        <span class="zone-label">Steering</span>
        <div class="track-h-wrap">
          <div class="track-h" id="steeringTrack">
            <div class="track-fill-h" id="steeringFill"></div>
            <div class="track-knob" id="steeringKnob"></div>
          </div>
        </div>
        <span class="zone-value" id="steeringVal">0&deg;</span>
      </div>
    </div>

  </main>

  <!-- Status Bar -->
  <footer class="status-bar">
    <div class="chip">Motor: <span id="motorVal">0</span></div>
    <div class="chip">Steer: <span id="steerVal">71</span>&deg;</div>
  </footer>

  <!-- Settings Modal -->
  <div id="settingsModal" class="modal-overlay">
    <div class="modal-sheet">
      <div class="modal-handle"></div>
      <div class="modal-header">
        <h2>Settings</h2>
        <button id="closeModalBtn" class="close-btn" type="button">&#10005;</button>
      </div>
      <div class="modal-body">
        <div class="setting-group">
          <div class="setting-head">
            <label for="servoCenterRange">Servo Center Angle</label>
            <strong><span id="servoCenterDisp">71</span>&deg;</strong>
          </div>
          <input id="servoCenterRange" type="range" min="0" max="180" step="1" value="71">
        </div>
        <div class="setting-group">
          <div class="setting-head">
            <label for="steerRangeRange">Steering Range</label>
            <strong>&plusmn;<span id="steerRangeDisp">49</span>&deg;</strong>
          </div>
          <input id="steerRangeRange" type="range" min="0" max="71" step="1" value="49">
          <div style="display:flex;justify-content:space-between;font-size:.75rem;color:#6b82a0;margin-top:4px">
            <span>Left: <span id="computedLeftDisp">120</span>&deg;</span>
            <span>Right: <span id="computedRightDisp">22</span>&deg;</span>
          </div>
        </div>
        <div class="setting-group">
          <div class="setting-head">
            <label for="maxSpeedRange">Max Motor Speed</label>
            <strong><span id="maxSpeedDisp">255</span></strong>
          </div>
          <input id="maxSpeedRange" type="range" min="0" max="255" step="1" value="255">
        </div>
      </div>
      <div class="modal-footer">
        <div id="settingsStatus" class="settings-status"></div>
        <button id="saveSettingsBtn" class="save-btn" type="button">Save Settings</button>
      </div>
    </div>
  </div>

  <!-- Toast -->
  <div id="toast" class="toast" role="status" aria-live="polite"></div>

  <script>
    /* ========== DOM References ========== */
    var $=function(id){return document.getElementById(id)};
    var statusDot=$('statusDot'),
        joyModeBtn=$('joyModeBtn'),sliderModeBtn=$('sliderModeBtn'),
        settingsBtn=$('settingsBtn'),
        joystickView=$('joystickView'),slidersView=$('slidersView'),
        joy=$('joy'),knob=$('knob'),
        joyXEl=$('joyXVal'),joyYEl=$('joyYVal'),
        tZone=$('throttleZone'),sZone=$('steeringZone'),
        tTrack=$('throttleTrack'),sTrack=$('steeringTrack'),
        tFill=$('throttleFill'),sFill=$('steeringFill'),
        tKnob=$('throttleKnob'),sKnob=$('steeringKnob'),
        tValEl=$('throttleVal'),sValEl=$('steeringVal'),
        motorValEl=$('motorVal'),steerValEl=$('steerVal'),
        modal=$('settingsModal'),closeModal=$('closeModalBtn'),
        saveBtn=$('saveSettingsBtn'),statusEl=$('settingsStatus'),
        scR=$('servoCenterRange'),srngR=$('steerRangeRange'),
        msR=$('maxSpeedRange'),
        scD=$('servoCenterDisp'),srngD=$('steerRangeDisp'),
        compLeftD=$('computedLeftDisp'),compRightD=$('computedRightDisp'),
        msD=$('maxSpeedDisp'),
        toastEl=$('toast');

    /* ========== State ========== */
    var mode='joystick';
    var normX=0,normY=0;
    var joyPtr=null,tPtr=null,sPtr=null;
    var lastMotor=-999,lastSteer=-999;
    var online=false;
    var toastTmr=0;
    var tAnimId=null,sAnimId=null;
    var ws;
    var wsConnected=false;
    var sendPending=false;
    var sendTimer=null;

    /* ========== Settings ========== */
    var cfg={servoCenter:71,servoLeftLimit:120,servoRightLimit:22,maxMotorSpeed:255};

    /* ========== Connection Status ========== */
    function setOnline(v){
      online=v;
      statusDot.classList.toggle('online',v);
      statusDot.classList.toggle('offline',!v);
    }

    function initWebSocket(){
      var url='ws://'+location.hostname+':81';
      console.log('WS connecting to', url);
      ws=new WebSocket(url);
      ws.onopen=function(){
        wsConnected=true;
        setOnline(true);
        console.log('WS connected');
      };
      ws.onclose=function(){
        wsConnected=false;
        setOnline(false);
        console.log('WS closed, reconnecting in 2s');
        setTimeout(initWebSocket, 2000);
      };
      ws.onerror=function(e){
        console.error('WS error', e);
      };
    }

    /* ========== Toast ========== */
    function toast(msg,type){
      clearTimeout(toastTmr);
      toastEl.textContent=msg;
      toastEl.className='toast '+type+' show';
      toastTmr=setTimeout(function(){toastEl.className='toast'},2500);
    }

    /* ========== Mode Switching ========== */
    function setMode(m){
      if(mode===m)return;
      mode=m;
      joyPtr=null;tPtr=null;sPtr=null;
      normX=0;normY=0;
      if(tAnimId){cancelAnimationFrame(tAnimId);tAnimId=null}
      if(sAnimId){cancelAnimationFrame(sAnimId);sAnimId=null}
      renderJoy();renderThrottle();renderSteering();
      joystickView.style.display=m==='joystick'?'flex':'none';
      slidersView.style.display=m==='sliders'?'flex':'none';
      joyModeBtn.classList.toggle('active',m==='joystick');
      sliderModeBtn.classList.toggle('active',m==='sliders');
      sendCtrl(true);
      try{localStorage.setItem('rc_mode',m)}catch(e){}
    }
    joyModeBtn.onclick=function(){setMode('joystick')};
    sliderModeBtn.onclick=function(){setMode('sliders')};

    /* ========== Mapping ========== */
    function mapSteer(n){
      var c=cfg.servoCenter,l=cfg.servoLeftLimit,r=cfg.servoRightLimit;
      if(n<=0)return Math.round(c+(-n/100)*(l-c));
      return Math.round(c+(n/100)*(r-c));
    }
    function mapMotor(n){return Math.round((n/100)*cfg.maxMotorSpeed)}

    /* ========== Control Sending ========== */
    function doSend(){
      var motor=mapMotor(normY);
      var steer=mapSteer(normX);
      motorValEl.textContent=String(motor);
      steerValEl.textContent=String(steer);
      if(motor===lastMotor&&steer===lastSteer)return;
      if(wsConnected && ws.readyState===WebSocket.OPEN){
        ws.send('C:'+motor+','+steer);
      }
      lastMotor=motor;lastSteer=steer;
    }
    function sendCtrl(force){
      if(force){
        doSend();
        sendPending=false;
        if(sendTimer){clearTimeout(sendTimer);sendTimer=null}
        return;
      }
      if(sendPending)return;
      sendPending=true;
      sendTimer=setTimeout(function(){sendPending=false;doSend()},25);
    }

    /* ========== Joystick Rendering ========== */
    function renderJoy(){
      var range=joy.clientWidth*0.33;
      var px=(normX/100)*range;
      var py=-(normY/100)*range;
      knob.style.transform='translate('+px+'px,'+py+'px)';
      joyXEl.textContent=String(normX);
      joyYEl.textContent=String(normY);
    }

    /* ========== Joystick Pointer ========== */
    function joyMove(e){
      var rect=joy.getBoundingClientRect();
      var cx=rect.left+rect.width/2,cy=rect.top+rect.height/2;
      var dx=e.clientX-cx,dy=e.clientY-cy;
      var maxR=rect.width*0.33;
      var dist=Math.hypot(dx,dy);
      if(dist>maxR){dx=(dx/dist)*maxR;dy=(dy/dist)*maxR}
      normX=Math.max(-100,Math.min(100,Math.round((dx/maxR)*100)));
      normY=Math.max(-100,Math.min(100,Math.round((-dy/maxR)*100)));
      renderJoy();sendCtrl();
    }
    joy.addEventListener('pointerdown',function(e){
      if(joyPtr!==null)return;
      e.preventDefault();joyPtr=e.pointerId;
      joy.setPointerCapture(e.pointerId);joyMove(e);
    });
    joy.addEventListener('pointermove',function(e){
      if(e.pointerId!==joyPtr)return;e.preventDefault();joyMove(e);
    });
    function releaseJoy(e){
      if(e.pointerId!==joyPtr)return;e.preventDefault();
      joyPtr=null;normX=0;normY=0;renderJoy();sendCtrl(true);
    }
    joy.addEventListener('pointerup',releaseJoy);
    joy.addEventListener('pointercancel',releaseJoy);
    joy.addEventListener('dragstart',function(e){e.preventDefault()});

    /* ========== Throttle Slider ========== */
    function tMove(e){
      var rect=tTrack.getBoundingClientRect();
      var cy=rect.top+rect.height/2,hh=rect.height/2;
      var dy=Math.max(-hh,Math.min(hh,cy-e.clientY));
      normY=Math.round((dy/hh)*100);
      if(tAnimId){cancelAnimationFrame(tAnimId);tAnimId=null}
      renderThrottle();sendCtrl();
    }
    tZone.addEventListener('pointerdown',function(e){
      if(tPtr!==null)return;e.preventDefault();
      tPtr=e.pointerId;tZone.setPointerCapture(e.pointerId);
      tZone.classList.add('active');tMove(e);
    });
    tZone.addEventListener('pointermove',function(e){
      if(e.pointerId!==tPtr)return;e.preventDefault();tMove(e);
    });
    function releaseT(e){
      if(e.pointerId!==tPtr)return;e.preventDefault();
      tPtr=null;tZone.classList.remove('active');animRet('t');
    }
    tZone.addEventListener('pointerup',releaseT);
    tZone.addEventListener('pointercancel',releaseT);

    /* ========== Steering Slider ========== */
    function sMove(e){
      var rect=sTrack.getBoundingClientRect();
      var cx=rect.left+rect.width/2,hw=rect.width/2;
      var dx=Math.max(-hw,Math.min(hw,e.clientX-cx));
      normX=Math.round((dx/hw)*100);
      if(sAnimId){cancelAnimationFrame(sAnimId);sAnimId=null}
      renderSteering();sendCtrl();
    }
    sZone.addEventListener('pointerdown',function(e){
      if(sPtr!==null)return;e.preventDefault();
      sPtr=e.pointerId;sZone.setPointerCapture(e.pointerId);
      sZone.classList.add('active');sMove(e);
    });
    sZone.addEventListener('pointermove',function(e){
      if(e.pointerId!==sPtr)return;e.preventDefault();sMove(e);
    });
    function releaseS(e){
      if(e.pointerId!==sPtr)return;e.preventDefault();
      sPtr=null;sZone.classList.remove('active');animRet('s');
    }
    sZone.addEventListener('pointerup',releaseS);
    sZone.addEventListener('pointercancel',releaseS);

    /* ========== Auto-Return Animation ========== */
    function animRet(which){
      var startVal=which==='t'?normY:normX;
      if(startVal===0){
        if(which==='t')renderThrottle();else renderSteering();
        sendCtrl(true);return;
      }
      var t0=performance.now(),dur=160;
      function frame(time){
        var p=Math.min((time-t0)/dur,1);
        var ease=1-Math.pow(1-p,3);
        var val=Math.round(startVal*(1-ease));
        if(which==='t'){
          normY=val;renderThrottle();
          if(p<1)tAnimId=requestAnimationFrame(frame);
          else{normY=0;renderThrottle();tAnimId=null}
        }else{
          normX=val;renderSteering();
          if(p<1)sAnimId=requestAnimationFrame(frame);
          else{normX=0;renderSteering();sAnimId=null}
        }
        sendCtrl(true);
      }
      if(which==='t'){
        if(tAnimId)cancelAnimationFrame(tAnimId);
        tAnimId=requestAnimationFrame(frame);
      }else{
        if(sAnimId)cancelAnimationFrame(sAnimId);
        sAnimId=requestAnimationFrame(frame);
      }
    }

    /* ========== Throttle Rendering ========== */
    function renderThrottle(){
      var v=normY;
      var kp=50-(v/100)*50;
      tKnob.style.top=kp+'%';
      if(v>=0){
        tFill.style.top=kp+'%';
        tFill.style.height=(50-kp)+'%';
        tFill.style.bottom='auto';
        tFill.className='track-fill-v fwd';
      }else{
        tFill.style.top='50%';
        tFill.style.height=(kp-50)+'%';
        tFill.style.bottom='auto';
        tFill.className='track-fill-v rev';
      }
      tValEl.textContent=v+'%';
    }

    /* ========== Steering Rendering ========== */
    function renderSteering(){
      var v=normX;
      var kp=50+(v/100)*50;
      sKnob.style.left=kp+'%';
      if(v>=0){
        sFill.style.left='50%';
        sFill.style.width=(kp-50)+'%';
      }else{
        sFill.style.left=kp+'%';
        sFill.style.width=(50-kp)+'%';
      }
      sValEl.textContent=mapSteer(v)+'\u00B0';
    }

    /* ========== Settings Modal ========== */
    settingsBtn.onclick=function(){
      modal.classList.add('active');syncForm();
    };
    closeModal.onclick=function(){modal.classList.remove('active')};
    modal.addEventListener('click',function(e){
      if(e.target===modal)modal.classList.remove('active');
    });

    function updateRangeMax(){
      var c=Number(scR.value);
      var maxRange=Math.min(c,180-c);
      srngR.max=maxRange;
      if(Number(srngR.value)>maxRange)srngR.value=maxRange;
      updateRangeDisp();
    }
    function updateRangeDisp(){
      var c=Number(scR.value),r=Number(srngR.value);
      srngD.textContent=r;
      compLeftD.textContent=c+r;
      compRightD.textContent=c-r;
    }
    function syncForm(){
      scR.value=cfg.servoCenter;
      var range=Math.round((cfg.servoLeftLimit-cfg.servoRightLimit)/2);
      if(range<0)range=-range;
      var maxRange=Math.min(cfg.servoCenter,180-cfg.servoCenter);
      srngR.max=maxRange;
      srngR.value=Math.min(range,maxRange);
      msR.value=cfg.maxMotorSpeed;
      scD.textContent=cfg.servoCenter;
      msD.textContent=cfg.maxMotorSpeed;
      updateRangeDisp();
      statusEl.textContent='';
    }
    scR.oninput=function(){scD.textContent=scR.value;updateRangeMax()};
    srngR.oninput=function(){updateRangeDisp()};
    msR.oninput=function(){msD.textContent=msR.value};

    function loadCfg(){
      fetch('/settings',{cache:'no-store'})
        .then(function(r){if(!r.ok)throw 0;return r.json()})
        .then(function(d){
          cfg.servoCenter=Number(d.servoCenter);
          cfg.servoLeftLimit=Number(d.servoLeftLimit);
          cfg.servoRightLimit=Number(d.servoRightLimit);
          cfg.maxMotorSpeed=Number(d.maxMotorSpeed);
          setOnline(true);
          renderSteering();
        })
        .catch(function(){});
    }

    saveBtn.onclick=function(){
      saveBtn.disabled=true;
      statusEl.textContent='Saving...';
      var center=Number(scR.value);
      var range=Number(srngR.value);
      var body=new URLSearchParams({
        servoCenter:center,
        servoLeftLimit:center+range,
        servoRightLimit:center-range,
        maxMotorSpeed:msR.value
      });
      fetch('/settings',{
        method:'POST',
        headers:{'Content-Type':'application/x-www-form-urlencoded'},
        body:body
      })
      .then(function(r){if(!r.ok)throw 0;return r.json()})
      .then(function(d){
        cfg.servoCenter=Number(d.servoCenter);
        cfg.servoLeftLimit=Number(d.servoLeftLimit);
        cfg.servoRightLimit=Number(d.servoRightLimit);
        cfg.maxMotorSpeed=Number(d.maxMotorSpeed);
        statusEl.textContent='';
        modal.classList.remove('active');
        toast('Settings saved','ok');
        setOnline(true);
        renderSteering();
      })
      .catch(function(){
        statusEl.textContent='Failed to save.';
        toast('Save failed','err');
      })
      .finally(function(){saveBtn.disabled=false});
    };

    /* ========== Visibility Change (safety stop) ========== */
    document.addEventListener('visibilitychange',function(){
      if(document.hidden){
        joyPtr=null;tPtr=null;sPtr=null;
        normX=0;normY=0;
        if(tAnimId){cancelAnimationFrame(tAnimId);tAnimId=null}
        if(sAnimId){cancelAnimationFrame(sAnimId);sAnimId=null}
        renderJoy();renderThrottle();renderSteering();
        sendCtrl(true);
      }
    });

    /* ========== Periodic Heartbeat ========== */
    setInterval(function(){
      if(normX!==0||normY!==0)doSend();
    },200);

    /* ========== Init ========== */
    setOnline(false);
    renderThrottle();
    renderSteering();
    try{if(localStorage.getItem('rc_mode')==='sliders')setMode('sliders')}catch(e){}
    loadCfg();
    initWebSocket();
  </script>
</body>
</html>
)=====";

#endif
