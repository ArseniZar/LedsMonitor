import { ScanNetworks } from "../../../../../basic/api/apiService";
import { html } from "../../../../../html";
import { Network } from "../../../../../basic/classes/Network";
import { useEffect, useState } from "preact/hooks";
import { NetworkItem } from "./NetworkItem";
import type { ResultData } from "../../../../../basic/api/types";
import {
  SignalLevel,
  type EncryptionType,
} from "../../../../../basic/types/Network";
import type { Callback } from "../../../../../basic/types/Callback";
import { Spin } from "../../../../basic/Spin";
import {
  fiveBarSignal,
  fourBarSignal,
  oneBarSignal,
  threeBarSignal,
  twoBarSignal,
} from "../../../../../basic/img/img";
import { Restart } from "../../../../basic/Restart";
import { Status } from "../../../../../basic/types/Step";

export function SearchNetwork(props: { selectNetworkEmit: Callback }) {
  const [networks, setNetworks] = useState<Network[]>([]);
  const [status, setStatus] = useState<Status> ();

  async function startSearchNetworks() {
    setStatus(Status.Loading);
    await searchNetworks();
    setStatus(Status.Ok);
  }

  async function searchNetworks() {
    const result: ResultData<Network[]> = await ScanNetworks();
    if (result.status) {
      setNetworks(
        result.value!.map(
          (item) =>
            new Network({
              ssid: item.ssid,
              password: item.password,
              rssi: item.rssi,
              encryptionType: item.encryptionType as EncryptionType,
              channel: item.channel,
              bssid: item.bssid,
              hidden: item.hidden,
            })
        )
      );
    }
  }

  function onSelect(id: string) {
    const network = findNetworkById(networks, id);
    if (network !== undefined) {
      props.selectNetworkEmit(network);
    }
  }

  function findNetworkById(
    networks: Network[],
    id: string
  ): Network | undefined {
    return networks.find((network) => String(network.getId()) === String(id));
  }

  function buildNetworkItem(item: Network) {
    const signalMap = {
      [SignalLevel.VeryWeak]: oneBarSignal,
      [SignalLevel.Weak]: twoBarSignal,
      [SignalLevel.Medium]: threeBarSignal,
      [SignalLevel.Strong]: fourBarSignal,
      [SignalLevel.VeryStrong]: fiveBarSignal,
    };

    const level = item.getSignalLevel();
    return item.toItemData(signalMap[level]);
  }

  useEffect(() => {
    startSearchNetworks();
  }, []);

  // prettier-ignore
  return html`
    <div class="w-full flex-1 flex flex-col items-center gap-3.5 px-6 py-4 mt-6 overflow-auto">
      ${status === Status.Ok        &&  html` ${networks.map(item => html`<${NetworkItem} item=${buildNetworkItem(item)} onSelectEmit=${onSelect} />`)} 
                                        <${Restart} onRestartEmit=${startSearchNetworks} />`}
      ${status === Status.Loading   &&  html`<${Spin}/>`}
    </div>
  `;
}
