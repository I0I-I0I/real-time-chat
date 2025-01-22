import { create } from "zustand"

interface UserStore {
    auth: boolean,
    login: string,
    username: string,
    setUser: (login: string, username: string) => void
    setAuth: (auth: boolean) => void
}

export const useUserStore = create<UserStore>()((set) => ({
    auth: false,
    login: "",
    username: "",
    setUser: (login: string, username: string) => set({ login, username }),
    setAuth: (auth: boolean) => set({ auth })
}))
