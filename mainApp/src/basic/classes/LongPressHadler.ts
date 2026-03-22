export class LongPressHandler {
  private isHolding = false;
  private holdTimer: ReturnType<typeof setTimeout> | null = null;
  private time: number;
  private callback: () => void;

  constructor(time: number, callback: () => void) {
    this.time = time;
    this.callback = callback;
  }

  handleLongPressStart() {
    this.isHolding = true;
    this.holdTimer = setTimeout(() => {
      if (this.isHolding) {
        this.callback();
      }
    }, this.time);
  }

  handleLongPressEnd() {
    this.isHolding = false;
    if (this.holdTimer) {
      clearTimeout(this.holdTimer);
      this.holdTimer = null;
    }
  }
}
