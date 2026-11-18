<div align="center">

<img src="docs/images/mfilm.png" width="112" alt="M-Film">

# M-Film

**Your Canon, shot like film.**

A film camera's interface for Canon cameras that run Magic Lantern.<br>
Load a roll from a shelf of 68 film stocks, shoot without looking at the screen, then develop the roll in the camera.<br>
The camera side is free and open, for everyone.

[![Build for EOS 7D](https://img.shields.io/badge/Build%20for%20EOS%207D-eca834?style=for-the-badge)](ml-dev/fork/7D.206/BUILD.md)
[![Buy the developer a coffee](https://img.shields.io/badge/Buy%20the%20developer%20a%20coffee-ffdd00?style=for-the-badge&logo=buymeacoffee&logoColor=black)](https://buy.stripe.com/7sY14p8d93QE5Ny5M58so02)

[![License GPL-2.0](https://img.shields.io/badge/license-GPL--2.0-3b352f)](LICENSE)
[![Cameras](https://img.shields.io/badge/cameras-10%20work%20%C2%B7%205%20with%20a%20problem%20%C2%B7%201%20in%20testing-3b352f)](#cameras)
[![Films](https://img.shields.io/badge/films-68-3b352f)](FILMS.md)

[![Instagram](https://img.shields.io/badge/Instagram-%40mfilm.official-3b352f?logo=instagram&logoColor=white)](https://www.instagram.com/mfilm.official/)
[![Threads](https://img.shields.io/badge/Threads-%40mfilm.official-3b352f?logo=threads&logoColor=white)](https://www.threads.com/@mfilm.official)
[![X](https://img.shields.io/badge/X-%40mfilm__official-3b352f?logo=x&logoColor=white)](https://x.com/mfilm_official)

**English** · [ภาษาไทย](README.th.md) · [Films](#68-films) · [Cameras](#cameras) · [Take it further](PORTING.md)

<br>

<img src="ml-dev/filmos/preview/shelf.gif" alt="The film shelf in the camera" width="720">

</div>

> [!WARNING]
> **Unofficial, experimental M-Film build for the Canon EOS 7D on firmware 2.0.6.**
> Build the 7D card files yourself from this source-only fork. It is maintained
> independently of the upstream M-Film and Magic Lantern projects and Canon.
> Earlier builds had limited testing on one camera.
> Use a spare CompactFlash card and back up your photos and settings first.
> Test builds can crash or lose data and are provided as is under the [GPL](LICENSE).
> Report 7D issues in [this fork's issue tracker](../../issues).

---

## A film camera, inside your camera

M-Film replaces the camera's menus with a film camera drawn from scratch, with Magic Lantern running
underneath. There is no picture on the screen after each shot and no going back through them: you shoot
the roll, then develop it and see what you got.

<table>
<tr>
<td width="50%"><img src="ml-dev/filmos/preview/shelf.png" alt="The film shelf"></td>
<td width="50%"><img src="ml-dev/filmos/preview/roll.png" alt="The roll"></td>
</tr>
<tr>
<td><b>Load a roll.</b> A shelf of canisters that turn to face you. Pick one and the leader comes out of it.</td>
<td><b>Count your frames.</b> A film camera's frame counter, from S to the last frame of the roll.</td>
</tr>
<tr>
<td><img src="ml-dev/filmos/preview/hud.png" alt="Viewfinder HUD"></td>
<td><img src="ml-dev/filmos/preview/developing.png" alt="Developing in the camera"></td>
</tr>
<tr>
<td><b>Shoot.</b> An SLR viewfinder over LiveView: aperture scale, light meter needle, red seven-segment frame count. Or one of three quieter ones.</td>
<td><b>Develop in the camera.</b> The darkroom develops the finished roll frame by frame, into JPEG folders on the card, at the size you choose.</td>
</tr>
<tr>
<td><img src="ml-dev/filmos/preview/prints-sheet.png" alt="Contact sheet"></td>
<td><img src="ml-dev/filmos/preview/home.png" alt="Home"></td>
</tr>
<tr>
<td><b>See the prints.</b> A contact sheet of the roll, each print full screen, and closer with the wheel.</td>
<td><b>Home</b> instead of the camera's menu: the roll you have loaded and everything else.</td>
</tr>
</table>

Every RAW is kept, so nothing is lost: you can develop the same roll again with another film later.

**Focusing a manual lens.** Loading a roll sets the camera to the film's ISO, and LiveView shows the exposure
the frame will get: with a slow film, or an adapted lens stopped down, it can be too dark to focus. Turn off
*Settings → Exposure simulation* and LiveView stays bright enough to focus, whatever the exposure.
(Not on the 600D, 1100D and 1300D: Magic Lantern can't set it there.)

### New in 0.2

**0.2.3** is about colour, on every camera:

- The darkroom develops a frame with the white balance the camera was on, and cuts it where that frame's own
  samples clip: blown highlights are no longer pink.
- The 59 films whose colour comes from Pat David's tables are fed a finished photo's tone, as those tables
  expect. Every colour film changes; pictures will not match the ones 0.2.1 developed.
- The glow round highlights is fainter on every film, and a highlight no longer goes red from its own light.
- The four cinema print films left the shelf: 68 film stocks.
- Every film stock says whether it is measured against the real film or only in its style
  ([FILMS.md](FILMS.md)). None is measured yet: [how a paired shot is made](docs/paired-shot.md).

**Copy the whole `ML` folder of the zip to the card**, not only `autoexec.bin`: the films' colour tables are
files in `ML/DARKROOM` now, and without them the darkroom stops with "no colour table".

**0.2.1** adds the [Canon EOS 1300D](TESTING.md#canon-eos-1300d), for testers. Nothing changes on the other cameras: they don't need to update.

<table>
<tr>
<td width="50%"><img src="ml-dev/filmos/preview/develop-size.png" alt="Choosing the print size before developing"></td>
<td width="50%"><img src="ml-dev/filmos/preview/prints-zoom.png" alt="Zoomed into a print"></td>
</tr>
<tr>
<td><b>Bigger prints.</b> Small, Medium or Large, asked before the darkroom starts: up to 2592 × 1728 from an 18 MP camera.</td>
<td><b>Zoom.</b> In the prints, turn the wheel to go into a frame and the arrows to move about.</td>
</tr>
<tr>
<td><img src="ml-dev/filmos/preview/hud-rangefinder.png" alt="The rangefinder viewfinder"></td>
<td><img src="ml-dev/filmos/preview/home.png" alt="Home"></td>
</tr>
<tr>
<td><b>Four viewfinders.</b> Classic (the SLR's), Minimal, Rangefinder and Clean: <i>Settings → Viewfinder</i>.</td>
<td><b>A shorter Home.</b> What to do next on top, then Darkroom and Settings. The battery is on every page.</td>
</tr>
</table>

- **Print sizes and zoom**: [how big the prints are](#how-big-the-prints-are).
- **A quieter darkroom.** The developing screen keeps the safelight, the tray and the paper, and draws again
  only when a print is done: the camera's one processor develops instead of animating. INFO dims the screen
  while it works.
- **The camera's white balance.** The darkroom develops each frame with the white balance the camera was on
  when it was shot. Loading a roll sets it to the light the film is made for: 5500 K, or 3200 K for a tungsten
  film. Change it by hand and the print follows. Daylight pictures come out cooler than they used to: the old
  fixed balance was too warm.
- **The 59 films whose colour comes from Film-Luts have more contrast and darker shadows.** Their tables were
  made by eye on finished photos, and the darkroom was giving them a flatter picture than any camera's own
  JPEG: about 88 % of its contrast. They now get the tone of a finished photo (Canon's Standard picture
  style), with the middle tones where they were. The eight films with M-Film's own model have not changed.
- **Four films have left the shelf:** the cinema print films Kodak Vision 2383 and 2393 and Fujifilm Eterna 3510
  and 3513. Their colour tables came with no licence, they were made for a film scan and not for a photo, and a
  print film is never loaded in a camera, so they could not be checked against the real thing. A roll already
  shot on one is listed in the darkroom under its name and is not developed as some other film; its RAWs stay
  on the card. If one of them is loaded when you update, rewind it first.
- **Date stamp** (*Settings → Date stamp*): the day in the corner of each print, in orange, like a compact's data back.
- **The battery** beside the clock on every page and in the viewfinder: in percent on the 60D, 6D, 70D,
  5D Mark II and 5D Mark III, whose battery reports it; as an icon with three levels on the others, where
  Canon knows no more.
- **The films you loaded last** have a shelf of their own, where the film shelf opens: the same film again is one press.
- **Settings** is one page (viewfinder, exposure simulation, print size, date stamp, film mode); the rest is
  under *Manual lens* and *Advanced*. The wind lever no longer locks the shutter: it reminds, or is off.
- **Menus that are simply there.** Going into a menu and back no longer slides; the fade between the shelf,
  the darkroom and the prints is shorter, and *Advanced → Page fade* turns it off.
- **The readout lights when you turn a dial**, not only on a half-press. On cameras with two dials (50D, 60D,
  70D, 5D Mark II, 5D Mark III, 6D) the top one changes shelves on the film shelf and the rear one picks the film.
- **EOS M: *Advanced → Hide Canon's graphics*** keeps Canon's exposure scale and icons off the viewfinder
  (its focus frame goes too, so it is off unless you turn it on).
- **One build.** The film OS now loads as a module after the camera starts on every camera, so Magic Lantern
  takes no more memory at power on than it does alone ([why](ml-dev/MEMORY.md)). On the cameras that had it
  built in until 0.1.6 (EOS M, 6D, 5D Mark III, 60D, 500D, 50D, 5D Mark II) this is new: see [Cameras](#cameras).

### How big the prints are

The camera's darkroom develops in three sizes. You choose when you start it (*INFO* on the Darkroom page
asks, and remembers) or in *Settings → Print size*:

| | Horizontal frame, from 18 MP | What it is | Zoom in the camera |
|---|---|---|---|
| **Small** | 720 × 480 (0.35 MP) | the camera's screen: the quickest | none |
| **Medium** | 1728 × 1152 (2 MP) | a third of the sensor each way | × 2.4 |
| **Large** | 2592 × 1728 (4.5 MP) | half the sensor each way | × 1.8 and × 3.6 |

- A vertical frame is the same, turned upright. Other sensors give in proportion: Large is 2736 × 1824 on a 6D,
  2880 × 1920 on a 5D Mark III.
- A bigger print takes longer: by the work it is, about 2.6 times a small one for Medium and 4.4 times for Large
  (on an EOS M with 0.2.3 a small one took 22 to 27 seconds a frame, and a Medium one about 50).
  The Darkroom page says how long before you start, from the frames the camera has developed at that size.
- The JPEG in `DCIM` is the size you chose. Medium and Large prints also keep a file to zoom into on the card
  (2 MB and 4.5 MB a frame).
- A camera without the memory for the size you asked makes the next one down, and says so.
- To make a roll's prints bigger later: *Delete → Prints only* on the Darkroom page, then develop it again.
  Your RAWs stay on the card.

### Basic or Realistic

<table>
<tr>
<td width="50%"><img src="ml-dev/filmos/preview/film-mode.png" alt="Film mode in Settings"></td>
<td width="50%"><img src="ml-dev/filmos/preview/end-of-roll.png" alt="A finished roll in Basic mode"></td>
</tr>
<tr>
<td><b>Choose how real.</b> <i>Settings → Film mode</i>.</td>
<td><b>Basic</b> (the default): the last frame finishes the roll, and it goes straight to the darkroom. No rewinding.</td>
</tr>
</table>

- **Basic:** shoot, then develop. Stop early with *Finish roll* on the roll's page (the first row of Home).
- **Realistic:** like a real film camera. Rewind the roll before it can be developed.

### On a DSLR: a top plate

<p align="center"><img src="ml-dev/filmos/preview/plate.png" alt="The top plate on a DSLR's back screen" width="720"></p>

A DSLR shoots through its optical viewfinder, so there is no LiveView to draw on. With a roll loaded and
LiveView off, the back screen becomes a film camera's top plate:

- shutter, aperture and frame in big seven-segment digits
- the film and its ISO
- a light meter from −2 to +2 stops, and a bar of the frames left
- WIND ON when the next frame needs winding

It goes off when you put your eye to the viewfinder, and LiveView brings the viewfinder HUD back.
*New in 0.1.1, still waiting for a DSLR tester: see [Cameras](#cameras).*

---

## 68 films

Colour negative, slide, black and white, and cine film, on four shelves. Each film has its own colour,
grain, halation and softness; rolls come in 36, 24 or 12 frames.

<!-- samples: docs/samples/<film>.jpg (3:2), made by ml-dev/films/pictures.py from ml-dev/films/sample/SAMPLE.CR2 -->
<table>
<tr>
<td width="33%"><img src="docs/samples/portra400.jpg" alt="Kodak Portra 400"><br><b>Kodak Portra 400</b><br><sub>Soft warm skin, gentle highlights</sub></td>
<td width="33%"><img src="docs/samples/gold200.jpg" alt="Kodak Gold 200"><br><b>Kodak Gold 200</b><br><sub>Golden hour, all day long</sub></td>
<td width="33%"><img src="docs/samples/ektar100.jpg" alt="Kodak Ektar 100"><br><b>Kodak Ektar 100</b><br><sub>Punchy colour, ultra fine grain</sub></td>
</tr>
<tr>
<td><img src="docs/samples/superia400.jpg" alt="Fujicolor Superia 400"><br><b>Fujicolor Superia 400</b><br><sub>Green shadows, everyday snaps</sub></td>
<td><img src="docs/samples/velvia50.jpg" alt="Fujichrome Velvia 50"><br><b>Fujichrome Velvia 50</b><br><sub>Vivid slide film, hard contrast</sub></td>
<td><img src="docs/samples/kodachrome64.jpg" alt="Kodak Kodachrome 64"><br><b>Kodak Kodachrome 64</b><br><sub>Magazine colour of the 70s</sub></td>
</tr>
<tr>
<td><img src="docs/samples/cinestill800t.jpg" alt="CineStill 800T"><br><b>CineStill 800T</b><br><sub>Tungsten cine film, night goes blue</sub></td>
<td><img src="docs/samples/pro400h.jpg" alt="Fujifilm Pro 400H"><br><b>Fujifilm Pro 400H</b><br><sub>Airy pastels, fourth colour layer</sub></td>
<td><img src="docs/samples/trix400.jpg" alt="Kodak Tri-X 400"><br><b>Kodak Tri-X 400</b><br><sub>Gritty contrast, street classic</sub></td>
</tr>
</table>

**[All 68 films, on the same photo →](FILMS.md)**

<!-- film-states:start -->
**None of the 68 film stocks is measured against the real film yet.** 65 are in the style of their film and not measured yet. 2 (Kodak HIE Infrared, Rollei Infrared 400) are impressions of infrared film: a Canon with its infrared filter in place does not record what that film does. 1 (CineStill 400T) is M-Film's own and stands for no real film. A film stock counts as measured when its colour and tone come within the pass marks of a lab scan of a roll of the real film, shot beside a Canon: [how a film stock becomes measured](docs/paired-shot.md). [Every film stock's state →](FILMS.md)
<!-- film-states:end -->

Colour for 59 of them comes from film emulation tables by [Pat David](https://patdavid.net),
[CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/), taken from G'MIC through the [Film-Luts](https://github.com/YahiaAngelo/Film-Luts)
collection; the rest from M-Film's own film models ([`ml-dev/films/films.py`](ml-dev/films/films.py)). The tables are
looks made by eye, not measurements of the films: [THIRD_PARTY.md](THIRD_PARTY.md#the-film-colour-tables-cc-by-sa-40).

---

## Cameras

M-Film was made on the EOS M, where it works. It now builds for every camera on which Magic Lantern
itself is mature (DIGIC 4 and 5). Testers have tried most of them now: the table says how far each one got, and what's
still wrong. If you have one marked *needs a tester*, you can help: [TESTING.md](TESTING.md) says how (about 20 minutes, and you can always go back).

<!-- cameras:start -->
| Camera | Canon firmware | Status | Notes | Card files |
|---|---|---|---|---|
| Canon EOS M | 2.0.2 | ✅ **Works** | From 0.2 the film OS loads as a module here too (as on the 550D and 650D), and it starts: tried with 0.2.3, rolls shot and developed in the camera. 0.2 can keep Canon's exposure scale off the viewfinder: *Advanced → Hide Canon's graphics* ([#67](../../issues/67)). | `M-Film-camera-EOSM.202.zip` |
| Canon EOS 5D Mark II | 2.1.2 | ⚠️ Tried, known problem | Most hang at power on: the build is too big at start-up ([#18](../../issues/18)). 0.1.3 has a smaller one (some Magic Lantern features left out, the darkroom kept) and fixes the pink prints: waiting for testers. From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-5D2.212.zip` |
| Canon EOS 5D Mark III (firmware 1.1.3) | 1.1.3 | ⚠️ Tried, known problem | Starts and shoots; the in-camera darkroom couldn't find the RAWs ([#31](../../issues/31)): fixed in 0.1.3. From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-5D3.113.zip` |
| Canon EOS 5D Mark III (firmware 1.2.3) | 1.2.3 | ⚠️ Tried, known problem | Starts and shoots; the in-camera darkroom couldn't find the RAWs ([#16](../../issues/16), [#37](../../issues/37)): fixed in 0.1.3. From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-5D3.123.zip` |
| Canon EOS 6D | 1.1.6 | ✅ **Works** | Works since 0.1.2 ([#17](../../issues/17), [#22](../../issues/22)). From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. 0.2 keeps Canon's screen away for as long as a frame is being saved, and lights the readout when a dial is turned ([#17](../../issues/17), [#23](../../issues/23)): say if either still happens. | `M-Film-camera-6D.116.zip` |
| Canon EOS 7D | 2.0.6 | 🧪 Experimental fork | Unofficial test build from this fork. Requires a bootable CompactFlash card. | [Build from source](ml-dev/fork/7D.206/BUILD.md) |
| Canon EOS 50D | 1.0.9 | ✅ **Works** | Works ([#43](../../issues/43)). 0.1.5 repaints the plate when Canon draws over it, but a slight flicker may remain, as on the 500D; 0.1.6 puts the plate together off-screen and puts back only what Canon drew over: reports wanted in [#64](../../issues/64). From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-50D.109.zip` |
| Canon EOS 60D | 1.1.1 | ✅ **Works** | Works since 0.1.3 ([#25](../../issues/25)). 0.1.5: Screen orientation for a broken hinge switch ([#49](../../issues/49)). From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-60D.111.zip` |
| Canon EOS 70D | 1.1.2 | ⚠️ Tried, known problem | 0.1.5 works on a tester's 70D, with the film OS as a module ([#40](../../issues/40)); on another, Magic Lantern installs but the camera then starts Canon's own firmware ([#58](../../issues/58)): more reports wanted. | `M-Film-camera-70D.112.zip` |
| Canon EOS 100D / Rebel SL1 / Kiss X7 | 1.0.1 | ✅ **Works** | Works, the film OS as a module since 0.1.5 ([#48](../../issues/48)). 0.1.6 as first released (8 Nov) flickered and took no buttons: fixed in the zips rebuilt on 9 Nov and in 0.2, and its tester says it works again ([#73](../../issues/73)). | `M-Film-camera-100D.101.zip` |
| Canon EOS 500D / Rebel T1i / Kiss X3 | 1.1.1 | ✅ **Works** | Works ([#44](../../issues/44)). 0.1.5 repaints the plate when Canon draws over it, but a slight flicker remains; 0.1.6 puts the plate together off-screen and puts back only what Canon drew over: does it still flicker? ([#64](../../issues/64)) From 0.2 the film OS loads as a module here too (as on the 550D and 650D): not confirmed on this camera yet, say if it starts. | `M-Film-camera-500D.111.zip` |
| Canon EOS 550D / Rebel T2i / Kiss X4 | 1.0.9 | ✅ **Works** | Works since 0.1.5, the film OS as a module, darkroom included ([#48](../../issues/48), [#50](../../issues/50)). | `M-Film-camera-550D.109.zip` |
| Canon EOS 600D / Rebel T3i / Kiss X5 | 1.0.2 | ✅ **Works** | Starts and shoots since 0.1.5, the film OS as a module ([#46](../../issues/46)). The in-camera darkroom isn't confirmed on it yet; shoot with Image quality RAW. | `M-Film-camera-600D.102.zip` |
| Canon EOS 650D / Rebel T4i / Kiss X6i | 1.0.4 | ✅ **Works** | Works with the film OS as a module, darkroom included ([#54](../../issues/54)). From 0.1.3 to 0.1.5 the plain zip came without the darkroom (with it built in, the 650D doesn't start: [#34](../../issues/34)): in 0.1.5, take the `-module.zip`. | `M-Film-camera-650D.104.zip` |
| Canon EOS 700D / Rebel T5i / Kiss X7i | 1.1.5 | ⚠️ Tried, known problem | 0.1.5's module build works on a tester's 700D, darkroom included ([#19](../../issues/19)), but M-Film's and Canon's menus still glitch over each other; 0.1.6 holds Canon's screen as the film OS opens, and as it closes back to the HUD: reports wanted. From 0.1.3 to 0.1.5 the plain zip came without the darkroom: in 0.1.5, take the `-module.zip`. | `M-Film-camera-700D.115.zip` |
| Canon EOS 1100D / Rebel T3 / Kiss X50 | 1.0.5 | ✅ **Works** | Works on testers' 1100Ds ([#81](../../issues/81), [#109](../../issues/109)): shooting, the frame count in LiveView (the optical viewfinder has nowhere to show one) and the in-camera darkroom at all three sizes, at about 11, 24 and 37 seconds a frame for Small, Medium and Large. Canon's newest firmware for it is 1.0.6, and M-Film needs 1.0.5: on 1.0.6, go back with Canon's 1.0.5 updater first ([#81](../../issues/81)), or *Firmware ver.* → update can stay on *Loading* ([TESTING.md](TESTING.md#if-firmware-ver--update-stays-on-loading)). | `M-Film-camera-1100D.105.zip` |
| Canon EOS 1300D / Rebel T6 / Kiss X80 | 1.1.0 | 🧪 Builds, needs a tester | New in 0.2.1, on Magic Lantern's own 1300D port, which is new and experimental. Installed with mlinstall over USB, not a firmware update ([TESTING.md](TESTING.md#canon-eos-1300d)). Loading a roll doesn't set ISO, white balance or picture style here: Magic Lantern can't change Canon's settings on the 1300D yet, so the look comes when the roll is developed. Set the camera's white balance to Daylight yourself (Tungsten for a tungsten film): the darkroom develops with the white balance the camera was on. Canon's firmware 1.1.0 comes in two kinds: if the camera hangs at power on, take the battery out and put the old card back. Needs a first tester. | `M-Film-camera-1300D.110.zip` |

**Not yet:** 5D Mark IV, 5DS R, 6D Mark II, 7D Mark II, 77D, 80D, 200D, 750D, 850D, M50, R, R5, R6, RP, SX70 HS, SX740 HS (Magic Lantern itself is still experimental on these).
<!-- cameras:end -->

✅ tried on the camera and works · ⚠️ tried, with a problem · 🧪 testing or experimental.
Build the 7D ZIP locally using the [7D build guide](ml-dev/fork/7D.206/BUILD.md).
For other models, see the
[upstream M-Film releases](https://github.com/HNGSeries/mfilm-canon-film-simulation/releases).
The camera list lives in [`ml-dev/cameras.txt`](ml-dev/cameras.txt) and porting notes in [PORTING.md](PORTING.md).

### Putting it on the camera

#### Canon EOS 7D (2.0.6)

1. Confirm the camera is on Canon firmware **2.0.6**. You need a camera set up
   to boot Magic Lantern and an already bootable **CompactFlash** card. See
   [the 7D Magic Lantern setup guide](https://github.com/ssh-den/magiclantern_simplified_7d/blob/dev/platform/7D.206/README.md)
   for the boot prerequisites.
2. [Build `M-Film-camera-7D.206.zip` from source](ml-dev/fork/7D.206/BUILD.md).
   Copy `autoexec.bin` and the `ML` folder from the same ZIP to the root of the
   CF card. Boot the camera with the card inserted.
3. Set Canon's **Image quality** to **RAW** or **RAW+JPEG**, and **Image review**
   to **Off**. Press **TRASH** to open M-Film. From Home, **MENU** returns to
   shooting and **Q** opens Canon Quick Control. The Magic Lantern menu is
   available in M-Film's Settings.
4. Leave Canon's file-name prefix at `IMG_` for the first test. M-Film reads
   the prefix reported by Canon for each shot, so a custom prefix belongs in the
   camera settings. If an older roll's journal does not match the file name,
   M-Film may find it for viewing or developing and will keep the original on
   the card when deleting.

For updates, finish your current roll, back up the card, and use the matching
`autoexec.bin` and `ML` files from the new build on a bootable CF card. The
standalone [7D Magic Lantern fork](https://github.com/ssh-den/magiclantern_simplified_7d)
can be used by itself.

#### Other models from upstream M-Film

1. The camera needs exactly the Canon firmware in the table (MENU → last settings tab → *Firmware ver.*).
2. Unzip the card files for your camera onto an SD card, put it in the camera and run *Firmware ver.* →
   update once. From then on M-Film starts with the camera from that card.
3. In Canon's menu set **Image quality** to **RAW** (RAW+JPEG works too) and **Image review** to **Off**.
   The darkroom develops the RAWs: a frame shot as JPEG can't be developed.
4. Full steps, and how to go back: [TESTING.md](TESTING.md). Building it yourself:
   `ml-dev/build_fork.sh <camera>`.

### Updating other models to a new version

Don't unzip a new version over the old one: files left from before can make the camera misbehave.
Start from a clean card instead:

1. **Finish the roll** and develop it in the camera. A new install starts with no roll loaded.
2. **Copy your photos off the card** (the `DCIM` folder), including the developed JPEG folders.
3. **Format the card in the camera** and choose to remove Magic Lantern.
4. **Unzip the new version** onto the card, as on the first install.
5. Put the card in the camera and run *Firmware ver.* → update again, then switch the camera on.

Your settings (film, screen colour, Film mode) go back to their defaults: set them again.

---

## Take it further

- **Have one of the cameras above?** [TESTING.md](TESTING.md): try it and tell us, so it can move to ✅.
- **Another camera?** [PORTING.md](PORTING.md) says what moves easily (the Film OS
  screens), what has to be redone per camera (the Magic Lantern side), and how to go about it.
- **Every screen and button** of the camera side, and how it works inside (in Thai):
  [docs/MANUAL.th.md](docs/MANUAL.th.md).
- **Something broken?** [Open an issue](../../issues/new/choose). shows.
- **The films** (looks, LUT list) are in [`ml-dev/films/`](ml-dev/films/); the camera build bakes them in.

## Support

The camera side of M-Film is free and stays free. If it gave you a roll you like, you can
**[buy the developer a coffee ☕](https://buy.stripe.com/7sY14p8d93QE5Ny5M58so02)**.

New versions and rolls people shoot with it are posted on
[Instagram](https://www.instagram.com/mfilm.official/), [Threads](https://www.threads.com/@mfilm.official)
and [X](https://x.com/mfilm_official).

## License

M-Film is made by **HNGSeries**, on top of [Magic Lantern](https://www.magiclantern.fm/).

This repository: GPL-2.0 ([LICENSE](LICENSE)), as the camera side builds on Magic Lantern.
The colour tables of
59 films are adapted from Pat David's and are CC BY-SA 4.0, not GPL-2.0. Those, the fonts and other
parts: [THIRD_PARTY.md](THIRD_PARTY.md). Not affiliated with Canon, Kodak, Fujifilm or any film maker.
