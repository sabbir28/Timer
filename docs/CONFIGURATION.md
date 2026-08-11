---
layout: page
title: "Configuration Reference"
permalink: /configuration.html
---

# Timer420 Configuration Reference

Settings are stored in `%APPDATA%\Timer420\timer420.ini`.

```ini
[Timer]
WorkSeconds=7200
BreakSeconds=600
OverlayMode=1
RepeatEnabled=1
RepeatCount=0

[Audio]
SoundEnabled=1

[Notifications]
ToastEnabled=1

[Window]
AlwaysOnTop=1
```

## Options

- `WorkSeconds`: Timer duration in seconds (5 to 86400).
- `OverlayMode`: `0` = Minimal, `1` = Progress, `2` = Edge Border.
- `RepeatEnabled`: `1` to auto-restart cycle on completion, `0` for single-shot.
- `RepeatCount`: `0` for infinite cycles, or N for fixed count.
- `SoundEnabled`: `1` to play completion sound alert.
- `ToastEnabled`: `1` to display Windows Toast notifications.
