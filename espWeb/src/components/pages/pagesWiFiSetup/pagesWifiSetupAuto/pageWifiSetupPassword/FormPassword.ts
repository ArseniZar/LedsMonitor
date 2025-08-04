import { useState } from "preact/hooks";
import { ped } from "../../../../../basic/img/img";
import type { Callback } from "../../../../../basic/types/Callback";
import { html } from "../../../../../html";

export function FormPassword(props: {addPasswordEmit: Callback, goBack: Callback}) {
  const [password, setPassword] = useState<string>("");

  function handleConfirm() {
    props.addPasswordEmit(password);
  }

  function handleInputPassword(e: Event) {
    const target = e.target as HTMLInputElement;
    setPassword(target.value);
  }
  
  // prettier-ignore
  return html`
    <div class="w-full flex-1 flex flex-col px-6 py-4 mt-6">
      <div class="">
        <button
          onClick=${props.goBack}
          class="p-1 rounded-full hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.5)] hover:scale-110 duration-300">
          <img src=${ped} alt="Check" class="h-6" />
        </button>
      </div>

      <div class=" w-full flex flex-col flex-1  gap-10  items-center justify-center -pt-20">
        <input
        type="text"
        onInput=${handleInputPassword}
        placeholder="Enter password"
        class="w-full p-3 text-sm font-mono bg-black text-white/90 rounded-2xl shadow-lg"
        />

        <button
          onClick=${handleConfirm}
          class="w-full   shadow-md  p-2 bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 transition block active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]">
          <span class="text-center text-md font-mono text-white/90 ">
            Confirm
          </span>
        </button>
      </div>
    </div>
  `;
}
