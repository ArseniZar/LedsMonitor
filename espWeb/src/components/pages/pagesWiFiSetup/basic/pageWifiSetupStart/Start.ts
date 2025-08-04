import { html } from "../../../../../html";
import type { Callback } from "../../../../../basic/types/Callback";

export function Start(props: {title: string, onStart: Callback }) {
  return html`
    <div class="w-full flex-1 flex flex-col px-10 items-center justify-center">
      <button onClick=${props.onStart} class="bg-black/40 p-2 w-full rounded-3xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]  active:bg-black hover:scale-105 transition">
        <span class="text-xl font-medium"> ${props.title} </span>
      </button>
    </div>
  `;
}
