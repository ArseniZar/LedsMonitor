import type { Network } from "../classes/Network";

export type ScanNetworksResponce = {
  networks: Network[];
};

export type ConnectNetworkRequest = {
  network: Network;
};

export type ConnectNetworkResponce = {
  status: { status: boolean };
};
