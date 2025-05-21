import styles from "./Message.module.css"
import cls from "@/utils/cls"

type Variants = "left" | "right"

interface MessageProps {
    children: React.ReactNode
    className?: string
    variant?: Variants
}

export const Message = ({ children, variant = "left", className = "" }: MessageProps) => (
    <li className={cls(styles.message, className, styles[variant])}>{children}</li>
)
