export type ApiSuccess<T>  = {
    ok: boolean;
    result: T;
};

export type ApiError  = {
    ok: false;
    error_code: number;
    description: string;
};

export type ResultData<T> = {
  status: boolean;
  value?: T;
  alertMessage?: string;
}