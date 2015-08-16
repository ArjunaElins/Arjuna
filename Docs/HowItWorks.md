How Arjuna Works
================

Arjuna communicates with keyboards by using MIDI (Musical Instrument Digital Interface) protocol, a protocol which enables digital musical instruments, computers, and other supported devices to communicate with each other. With this protocol, MPU can receive data such as notes and velocity from the keyboard while the student is playing.

## The Two Modes

Arjuna has two modes of operation, the *Listen* and the *Evaluation* mode. Both modes require two sets of data, one for song and one for fingering data. Songs are stored in MIDI file (.mid), so you can get it from anywhere on the internet, or you can even make your own song with any MIDI song creator (there are a lot out there). Fingering data can be created from the song file with a software we called MidiFGR. In this software, you can manually set the correct finger to play for each notes, and then it will generate a new file. This file is needed to guide the student to play the correct key with the correct finger.

#### Listen Mode

On Listen mode, MPU will read the chosen song and tell the keyboard to play that song. On the same time, the MPU will send commands to the hand modules, and it will vibrate the correct finger for every notes that should be played. This way, the student can learn the proper fingering skill.

#### Evaluation Mode

On Evaluation mode, the student can play the keyboard and receive the guidance. While the student is playing, the MPU evaluates data received from the keyboard by comparing it with the song file. Once the student made a mistake, MPU will detect it and a command will be given to the hand module. Then the hand module will vibrate the motor on the left side or the right side of the correct finger, depending on the position of the key that should be pressed relative to the wrong pressed key. If the correct key is located to the right of the wrong pressed key, the left motor will vibrate. On the other hand, if the correct key is located to the left of the wrong pressed key, the right motor will vibrate. Of course this behavior can be reversed on the MPU settings.