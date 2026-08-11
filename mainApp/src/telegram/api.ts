import { HttpError, InvalidJsonError, NetworkError } from './errors'
import type { User, Chat, ChatMember, ApiError, ApiSuccess, Message, Update } from '@telegraf/types';


export async function getBotInfo(token: string): Promise<User> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getMe`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<User> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}





export async function getChatInfo(token: string, chatId: string): Promise<Chat> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getChat?chat_id=${chatId}`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<Chat> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function getChatMemberInfo(token: string, chatId: string, userId: string): Promise<ChatMember> {
  const response: Response = await fetch(`https://api.telegram.org/bot${token}/getChatMember?chat_id=${chatId}&user_id=${userId}`).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<ChatMember> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function postCommandToTelegramChat(token: string, chatId: string, text: string): Promise<Message> {
  const response = await fetch(`https://api.telegram.org/bot${token}/sendMessage`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      chat_id: chatId,
      text: text
    })
  })
    .catch(() => {
      throw new NetworkError('Network error occurred while fetching updates');
    });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<Message> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}



export async function getTelegramUpdates(token: string, offset?: number, allowed_updates?: string[]): Promise<Update[]> {
  const url = new URL(`https://api.telegram.org/bot${token}/getUpdates`);

  if (offset !== undefined) {
    url.searchParams.set('offset', String(offset));
  }

  if (allowed_updates !== undefined) {
    url.searchParams.set('allowed_updates', JSON.stringify(allowed_updates));
  }

  const response: Response = await fetch(url.toString()).catch(() => {
    throw new NetworkError('Network error occurred while fetching updates');
  });


  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => { throw new InvalidJsonError(); });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<Update[]> = await response.json().catch(() => { throw new InvalidJsonError(); });
  return body.result;
}






