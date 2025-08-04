import { html } from "../../html";

export function Spin() {
  return html`
    <div
      class="w-5 h-5 border-3 mx-auto border-transparent border-t-white/90 border-r-white/90 border-b-white/90 rounded-full animate-spin"
    ></div>
  `;
}
