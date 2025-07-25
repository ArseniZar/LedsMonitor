import mitt from 'mitt';

type Events = {
  alert: string;
};

const emitter = mitt<Events>();

export default emitter;