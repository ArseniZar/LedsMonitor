import { useEffect, useState } from "preact/hooks";
import { Network } from "../../../../../basic/classes/Network";
import { device, router } from "../../../../../basic/img/img";
import { Status } from "../../../../../basic/types/Step";
import { html } from "../../../../../html";
import { End, ConnectNetwork } from "../../../../../basic/api/apiService";
import type { ResultData } from "../../../../../basic/api/types";
import type { Callback } from "../../../../../basic/types/Callback";

// prettier-ignore
export function Connect(props: { network: Network, connectedEmit: Callback, goBack: Callback }) {
  const [status, setStatus] = useState<Status>();

  async function startConnect() {
    setStatus(Status.Loading);
    const status = await Connect(props.network);
    setStatus(status ? Status.Ok : Status.Error);
  }

  async function Connect(network: Network): Promise<boolean> {
    const result: ResultData<boolean> = await ConnectNetwork(network);
    return result.status && result.value as boolean;
  }


  function hadleConnected(){
    End();
    props.connectedEmit();
  }

  function renderAnimation(props: { status: Status }) {
    const delays = [0, 150, 300, 450, 600];
    const isLoading = props.status === Status.Loading;
    const colorClass =
      props.status === Status.Ok
        ? "text-green-400"
        : props.status === Status.Error
        ? "text-red-600"
        : "text-white";
    // prettier-ignore
    return html`
      <div class="flex gap-1 ${colorClass} text-2xl font-bold leading-none">
        ${delays.map(
          (delay) =>
            html`
                <span class=${isLoading? `animate-bounce [animation-delay:${delay}ms]` : ""}>
                    .
                </span>
            `)
        }
      </div>
    `;
  }

  function renderButton(props: { title: string; onClick: Callback }) {
    // prettier-ignore
    return html` 
        <button onClick=${props.onClick} class="w-full shadow-md p-2 bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 transition active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]">
            <span class="text-center text-md font-mono text-white/90 ">
                ${props.title}
            </span>
        </button>
    `;
  }

  function renderText(props: { title: string }) {
    // prettier-ignore
    return html` 
        <span class="text-gray-300 text-center font-mono text-md select-none block">
            ${props.title}
        </span>
    `;
  }

  useEffect(() => {
    startConnect();
  }, []);

  // prettier-ignore
  return html` 
    <div class="w-full flex-1 flex flex-col items-center px-6 py-4 justify-center select-none">
        <div class="w-full">
            ${status === Status.Loading && html` <${renderText} title=${"Connecting... Please do not refresh"} />`}
            ${status === Status.Ok && html` <${renderText} title=${"Successfully connected"} />`} 
            ${status === Status.Error && html` <${renderText} title=${"Failed to connect to the network"} />`}
        </div>
        <div class="w-full flex flex-1 flex-row items-center justify-center gap-4 py-6">
            <img src=${device} alt="Device" class="h-14 w-14" />
            <${renderAnimation} status=${status} />
            <img src=${router} alt="Router" class="h-14 w-14" />
        </div>
        <div class="w-full min-h-[2.5rem] flex flex-row gap-1">
            ${status === Status.Ok && html` <${renderButton} title=${"End"} onClick=${hadleConnected} />`} 
            ${status === Status.Error && html` <${renderButton} title=${"Back"} onClick=${props.goBack} />`}
            ${status === Status.Error && html` <${renderButton} title=${"Reload"} onClick=${startConnect} />`}
        </div>
    </div>
  `;
}
