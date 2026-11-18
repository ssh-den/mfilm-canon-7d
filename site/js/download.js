// Select an upstream camera ZIP.

import { T } from './i18n.js';
import { latestRelease } from './releases.js';

// the cameras with a release zip, M-Film-camera-<platform>.zip: fos/cameras.json (from ml-dev/cameras.txt)
// once it has loaded; this list until then, or if it can't be
let CAMERAS = [
  ['EOSM.202', 'Canon EOS M', 'working'],
  ['6D.116', 'Canon EOS 6D', 'working'],
  ['60D.111', 'Canon EOS 60D', 'working'],
  ['50D.109', 'Canon EOS 50D', 'working'],
  ['500D.111', 'Canon EOS 500D / Rebel T1i / Kiss X3', 'working'],
  ['550D.109', 'Canon EOS 550D / Rebel T2i / Kiss X4', 'working'],
  ['600D.102', 'Canon EOS 600D / Rebel T3i / Kiss X5', 'working'],
  ['100D.101', 'Canon EOS 100D / Rebel SL1 / Kiss X7', 'working'],
  ['650D.104', 'Canon EOS 650D / Rebel T4i / Kiss X6i', 'working'],
  ['1100D.105', 'Canon EOS 1100D / Rebel T3 / Kiss X50', 'working'],
  ['700D.115', 'Canon EOS 700D / Rebel T5i / Kiss X7i', 'issues'],
  ['70D.112', 'Canon EOS 70D', 'issues'],
  ['5D3.113', 'Canon EOS 5D Mark III (firmware 1.1.3)', 'issues'],
  ['5D3.123', 'Canon EOS 5D Mark III (firmware 1.2.3)', 'issues'],
  ['5D2.212', 'Canon EOS 5D Mark II', 'issues'],
  ['1300D.110', 'Canon EOS 1300D / Rebel T6 / Kiss X80', 'testing'],
];
const REPO = 'https://github.com/HNGSeries/mfilm-canon-film-simulation';
let camTag = null;
const tagReady = latestRelease((t) => /^v\d/.test(t)).then((r) => { camTag = r && r.tag; });
const zipUrl = (p) => camTag ? `${REPO}/releases/download/${camTag}/M-Film-camera-${p}.zip`
                             : `${REPO}/releases/latest/download/M-Film-camera-${p}.zip`;
const settled = () => Promise.race([tagReady, new Promise((r) => setTimeout(r, 2500))]);

const dlg = document.getElementById('dl');
const sel = document.getElementById('dl-camera');
const note = document.getElementById('dl-camera-note');
const go = document.getElementById('dl-go');

function fillCameras() {
  const keep = sel.value || remembered() || 'EOSM.202';
  sel.innerHTML = '';
  for (const [p, name, status] of CAMERAS) {
    const o = document.createElement('option');
    o.value = p;
    o.textContent = status === 'working' ? name : `${name} · ${T('dl.' + status)}`;
    sel.append(o);
  }
  sel.value = keep;
  showNote();
}
function showNote() {
  const c = CAMERAS.find(([p]) => p === sel.value);
  note.textContent = c && c[2] !== 'working' ? T('dl.note.' + c[2]) : '';
  try { localStorage.setItem('mfilm.camera', sel.value); } catch {}
}
function remembered() { try { return localStorage.getItem('mfilm.camera'); } catch { return null; } }

function startDownload(p) {
  const a = document.createElement('a');
  a.href = zipUrl(p);
  a.rel = 'noopener';
  document.body.append(a);
  a.click();
  a.remove();
}

async function thanks(p) {
  dlg.querySelector('.dl-ask').hidden = true;
  dlg.querySelector('.dl-thanks').hidden = false;
  const again = document.getElementById('dl-again');
  again.href = zipUrl(p);
  open();
  await settled();
  again.href = zipUrl(p);
  startDownload(p);
}

function open() {
  if (!dlg.open) dlg.showModal();
}

async function submit() {
  await thanks(sel.value);
}

// ---------------------------------------------------------------- wiring

fillCameras();
fetch('fos/cameras.json').then((r) => r.json()).then((j) => {
  CAMERAS = j.cameras.map((c) => [c.platform, c.name, c.status === 'nodark' ? 'working' : c.status]);
  fillCameras();
}).catch(() => {});
sel.addEventListener('change', showNote);
go.addEventListener('click', submit);
dlg.addEventListener('click', (e) => { if (e.target === dlg) dlg.close(); });   // outside the box
dlg.addEventListener('close', () => {
  dlg.querySelector('.dl-ask').hidden = false;
  dlg.querySelector('.dl-thanks').hidden = true;
});
for (const a of document.querySelectorAll('[data-download]')) {
  a.addEventListener('click', (e) => { e.preventDefault(); open(); });
}
document.addEventListener('mfilm:lang', fillCameras);

// install.html's Download button, and links to #get: the dialog, open
if (location.hash === '#get') { open(); history.replaceState(null, '', location.pathname + location.search + '#download'); }
addEventListener('hashchange', () => { if (location.hash === '#get') open(); });
