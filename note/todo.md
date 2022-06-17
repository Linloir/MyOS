# TODOs

## Page Manager

- mapPage() should retrieve table from caller and request frame from frame manager instead of caller request for frame and pass to page manager

## Process

- Process should have its own heap / heap management
- Better way for process to choose its stack addr instead of emptySlot() ?
- space malloced but not freed will leak when process ends
